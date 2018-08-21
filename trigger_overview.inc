\section{Trigger Infrastructure Overview}
Our trigger infrastructure is implemented in Ascent~\cite{Larsen:2017:ASI:3144769.3144778}, a fly-weight in situ visualization and analysis infrastructure.
%
Ascent's flexibility is underpinned by a generic data-flow network.
%
Filters in the data-flow network have arbitrary inputs and outputs, and each component of Ascent is implemented as a filter.
%
The Ascent runtime translates data and actions described in the high level API into a graph that is executed by the data-flow network.
%
Using this modular architecture, all of Ascent's components can be connected together.
%
The trigger implementation in Ascent is simply another filter in a larger data-flow network that can conditionally modify the filters in the execution graph.

\fix{In this section, we discuss what types of simulation data are used to make decisions, how decisions are made, and what types of actions are supported.}


\subsection{Trigger Input}

% \fix{Ascent triggers make decisions using Mesh-Blueprint data.
% In addition to the fact that results from pipelines can be feed in (so we can reuse VAS actions) - the blueprint angle is important high-level point -- we can pull from next section}

Ascent's trigger filters process Conduit\cite{conduit} data that conforms to the Mesh Blueprint\cite{conduitblueprint}. This is the same data representation used to publish data to Ascent.  Since outputs from Ascent operations can also be converted to Mesh Blueprint data, this representation allows triggers to operate on Ascent results as well.  The triggers have access to all of the mesh data, however its useful to further categorize triggers by the parts of simulation mesh required by inspection routines.
%
In Ascent we define following categories of triggers:
\begin{itemize}
\item \textbf{Field}: inspect a specific field on the mesh
\item \textbf{Topology}: inspect a specific mesh topology
\item \textbf{Coordinate Set}: inspect a specific coordinate set from the mesh
\item \textbf{State}: inspect state metadata 
\end{itemize}

Field triggers have access to all the values of a named field on the mesh.
%
A simple example of a field trigger is one that fires when the maximum value exceeds a user defined threshold.
% 
% need to rework these sentences english wise
Topology triggers are passed a named mesh topology. An example of topology trigger would inspect the mesh, firing when a tangle is detected.
%
% need to rework these sentences english wise
Coordinate Set triggers are passed a named coordinate set. An example of coordinate set trigger would inspect the coordinates, firing when a bounding box grows to a given size.
%
State triggers consume extra metadata associated with the mesh. 
This includes both basic information like the simulation time and cycle, and custom data such as simulation performance metrics.  
%
Examples of state triggers are fire every $X$ cycles or fire when a performance metric rises above some critical threshold.
%

Categorizing triggers is useful for declaring a trigger's purpose (i.e., what data is used to make decisions) and to share common  parameter verification code that checks pre-conditions for each category.

%
%Our trigger infrastructure supports combining types, enabling the creation more complex inspection routines.

 
\subsection{Trigger Decisions}

Triggers make a binary (yes/no) decision to execute a set conditional set of actions. This process involves data reduction and and a final decision to fire.

\subsubsection{Data Reduction}
%Triggers need to be cheap. They need to be useful. 

The first step in the decision process is to reduce the input simulation data. The data reduction may leverage a pipeline of existing VAS actions, or it may be a simple data summarization algorithm. While triggers can leverage expensive algorithms, in practice they are often used to reduce overall computation by screening data before applying more expensive VAS actions. 
Because of this, inexpensive data reduction techniques are important. 


%From a user's perspective there are a few common constructs that are both cheap and useful. 
The most basic technique is a simple aggregation of a field or a topological feature (minimum, maximum, mean, variance, integral, etc).
These are inexpensive because they do not require much intermediate memory, are typically simple linear loops over values and their distributed-memory implementations are straightforward and scalable.
%
In Ascent, we provide a small set of methods that serve as building blocks for trigger development and encourage code re-use.
%
Current methods include retrieving the minimum and maximum values of a field, along with the element or vertex it originated from, and the element's location.
%
As we continue to develop Ascent, the set of available methods will increase.

A more sophisticated way to reduce data is to calculate a coarse distribution of a field or a topological feature. This can be calculated inexpensively with a counting or weight-based histogram. Calculating exact cumulative distributions requires an expensive global sort, so we do not plan to implement these. Multi dimensional binning can also be used to calculate coarse averages which can be overlaid back onto the mesh topology as input to create more complex derived quantities for summarization. \cite{ecf07} For distributions, you can further reduce them using another summary metric, such as identifying percentiles or calculating the shannon entropy. \fix{We have a prototype approach to calculate distributions and further summarize them.}

\subsubsection{Decision to fire}

The next step in the decision process is to apply a final test to determine if the trigger should fire. 

The Ascent trigger infrastructure allows two trigger variations:
\begin{itemize}
\item \textbf{Stateless}: trigger execution is independent of any previous invocations
\item \textbf{Stateful}: the trigger that maintains state information about previous invocations
\end{itemize}

Stateless decisions are made using only the simulation's current published data, and stateful decisions can use the time history of the aggregate values from previously published data. 
%
In the stateless case, the standard menu of numeric comparison operations (e.g. greater-than, less-than, equal, etc) are useful building blocks for deciding to fire using a single value. 
%
For the stateful case, trigger firing can be based on critical points of the curve (minimum, maximum, saddle points),  or leverage a  more complex property, such as the value changing more than some percentage of the current peak. 
%
%You may need a smoothing function to make robust decisions about a time history curve. 
%
In Ascent we provide methods for storing and retrieving arbitrary state data, and state data can range from a single value, a series of values from all previous time steps, or to the entire published data from a previous time step.


\subsection{Trigger Actions}
Ascent's interface supports execution of a set of actions described by the user. 
%
These actions direct Ascent to create pipelines (i.e., transform data), extracts (i.e., capture data), and  scenes (i.e., make pictures). The new trigger infrastructure extends Ascent's interface to accept trigger declarations.

The declaration accepts a trigger name, any input parameters needed for the inspection routine, and a set of 
actions to execute if the trigger fires. 
%
The set of actions is declared using same Ascent interface, thus exposing all of Ascent's capabilities including declaring new triggers.

%
Examples of trigger actions are saving the entire mesh to disk, creating a pipeline to transform the data and saving the resulting extract, or rendering images. 
%
Additionally, trigger actions can include other triggers that create a set of conditions (contained in multiple triggers) which all must be satisfied before the final set of actions is performed.
%
%Initially, triggers are a sink in the data flow network. If the trigger fires, the sink is converted into an inner node of %the graph, and the data flow network is extended based on the actions provided  by the user. 
Since triggers have access to the input actions, it is possible for triggers to themselves modify the actions, creating dynamically adaptive visualization workflows.
%

\subsection{Trigger API}
Our trigger API matches the rest of Ascent and triggers are described using conduit nodes.
%
Listing~\ref{triggerAPI} shows an example of specifying a stateless threshold trigger of the field pressure that fires when the condition is true.
%
\begin{lstlisting}[caption={An example of the user-facing trigger API. This triggers executes the actions when the maximum value of the pressure field is greater than or equal to 3.14.},captionpos=b, label={triggerAPI}]
conduit::Node actions;
// trigger actions not shown
conduit::Node trigger;
trigger["type"] = "threshold";
trigger["params/field"] = "pressure";
trigger["params/actions"] = actions;
trigger["params/reduction"] = "max";
trigger["params/compare/type"] = "gte";
trigger["params/compare/value"] = 3.14;
\end{lstlisting}
The threshold triggers supports several reductions including ``min'', ``max'', and ``average''.
%
As we continue to develop the infrastructure, the menu of available reduction operations will increase.
%
After the reduction, the trigger evaluates the condition ``gte'', greater than or equal to, against the ``value'' and the result of the reduction.
%


\iffalse
\subsection{Implementation}
Triggers functionality in Ascent are implemented in two classes.
%
The trigger base class has a number of responsibilities including providing access to input data, validating parameters (e.g., actions), calling a derived class's trigger function, and executing the actions if the trigger fires. 
%
Derived triggers initialize the base class to register what types of data require for its inspection routine.
%
The only function a derived trigger is required to implement is the inspection routine.
%
Stateful triggers inherit from both the trigger base type and the Stateful class, which contains the facilities to save and retrieve state information. 
Listings~\ref{baseclass} and ~\ref{stateclass}

Triggers can be defined in the host code (domain-specific), directly into ascent (domain-agnostic), or exist as a python script on the file system (both). That said, one of the goals of the trigger infrastructure design is to encourage the sharing and re-use of different triggers.

%\begin{figure}
%\includegraphics[width=6cm]{images/inheritance}
%\caption{\label{inheritance} An inheritance diagram for the Ascent trigger infrastructure of a custom mesh trigger.}
%\end{figure}

\begin{lstlisting}[caption={The trigger base class API. Each derived trigger type implements the ``get\_data'' method that retreives the appopriate topology, field, or state data from the input.},captionpos=b, label={baseclass}]
class Trigger : public Filter
{
public:
  virtual bool trigger() = 0;
  void exectute()
  {
  	bool fire = trigger();
  	if(fire)
  	{
  	  // execute actions
  	} 
  }
protected:
  const conduit::Node& get_field();
  const conduit::Node& get_topology();
  const conduit::Node& get_get_coords();
  const conduit::Node& get_state();
  conduit::Node actions;
};

\end{lstlisting}

\begin{lstlisting}[caption={The trigger state base class that provides methods for setting and retrieving state inforation stored in conduit nodes. State data is stored in a static key-value store accessible to all trigggers },captionpos=b,label={stateclass}]
class TriggerState
{
protected:
 conduit::Node& get_state(std::string key);
 void set_state(std::string key, conduit::Node &state);
};

\end{lstlisting}
\fi