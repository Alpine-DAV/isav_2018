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
The trigger implementation in Ascent is simply another filter in a larger data-flow network that can dependently create further nodes in the execution graph.

\fix{In this section, we discuss what types of simulation data are used to make decisions, how decisions are made, and what types of actions are supported.}


\subsection{Trigger Input}

% \fix{Ascent triggers make decisions using Mesh-Blueprint data.
% In addition to the fact that results from pipelines can be feed in (so we can reuse VAS actions) - the blueprint angle is important high-level point -- we can pull from next section}

Ascent's trigger filters process Conduit\cite{} data that conforms to the Mesh Blueprint\cite{}. This is the same data representation used to publish data to Ascent.  Since the results Ascent operations can also be converted to Mesh Blueprint data, this representation allows triggers to operate on Ascent results as well.  The triggers have access to all of the mesh data, however its useful to further categorize triggers by the parts of simulation mesh required by the evaluation routine.
%
In Ascent we define following categories of triggers:
\begin{itemize}
\item \textbf{Field}: examine a specific field on the mesh
\item \textbf{Topology}: examine a specific mesh topology
\item \textbf{Coordinate Set}: examine a specific coordinate set from the mesh
\item \textbf{State}: examine state metadata 
\end{itemize}

Field triggers have access to all the values of a named field on the mesh.
%
A simple example of a field trigger is one that fires when the maximum value exceeds a user defined threshold.
% 
% need to rework these sentences english wise
Topology triggers are passed a named mesh topology. An example of topology trigger would examine the mesh, firing when a tangle is detected.
%
% need to rework these sentences english wise
Coordinate Set triggers are passed a named coordinate set. An example of coordinate set trigger would examine the coordinates, firing when a bounding box grows to a given size.
%
State triggers consume extra metadata associated with the mesh. 
This includes both basic information like the simulation time and cycle, and custom data such as simulation performance metrics.  
%
Examples of state triggers are fire every $X$ cycles or fire when a performance metric rises above some critical threshold.
%

Categorizing triggers is useful for declaring a trigger's purpose (i.e., what data is used to make decisions) and to share common  parameter verification code that checks pre-conditions each category.

%
%Our trigger infrastructure supports combining types, enabling the creation more complex inspection routines.

 
\subsection{Trigger Decisions}

Two Parts: Data Reduction and Decide to Fire.

\subsubsection{Reduce the data}
%Triggers need to be cheap. They need to be useful. 

The first step in the decision process is to reduce the input simulation data. The data reduction may leverage a pipeline of existing VAS actions, or it may be a simple data summarization algorithm. While triggers can leverage expensive algorithms, in practice they are often used to reduce overall computation by screening data before applying more expensive VAS actions. 
Because of this, inexpensive data reduction techniques are important. 


%From a user's perspective there are a few common constructs that are both cheap and useful. 
The most basic technique is a simple aggregation of a field or a topological feature (minimum, maximum, mean, variance, integral, etc).
These are inexpensive because they do not require much intermediate memory, are typically  O(number\_of\_elements) and their distributed-memory implementations are straightforward and scalable.
%
In Ascent, we are provide a small set of methods that will serve as the building blocks for trigger development and encourage code re-use.
%
Current methods include retrieving the min and max values of a field, along with the cell or point it originated from, and getting a cells location.
%
As we continue to develop Ascent, the set of available methods will increase.

A more sophisticated way to reduce data is to calculate a coarse distribution of a field or a topological feature. This can be calculated inexpensively with a counting or weight-based histogram. (mention: exact or cumulative distribution would require a sort, which is not cheap). Multi dimensional binning can also be used to calculate coarse averages which can be overlaid back onto the mesh topology as input to create more complex derived quantities for summarization. \cite{ecf07} For distributions, you can further reduce them using another summary metric, such as identifying percentiles or calculating the shannon entropy. 

\subsubsection{Decide to fire}

The next step in the decision process is to apply a final test to determine if the trigger should fire. 

The Ascent trigger infrastructure allows two trigger variations:
\begin{itemize}
\item \textbf{Stateless}: trigger execution is independent of any previous invocations
\item \textbf{Stateful}: the trigger that maintains state information about previous invocations
\end{itemize}

Stateless decisions can be are made on the simulation's current aggregate value, and stateless decisions using the time history of the aggregate values from previously published data. 
%
In the stateless case, the standard menu of numeric comparison operations (e.g. greater-than, less-than, equal, etc) are useful building blocks for deciding to fire using a single value. 
%
For the stateful case, trigger firing can be based on critical points of the curve (minimum, maximum, saddle points),  or about a more complex property, such as the value changing more than some percentage of the current peak. 
%
%You may need a smoothing function to make robust decisions about a time history curve. 
%
In Ascent we provide methods for storing and retrieving arbitrary state data, and state data can range from a single value, a series of values from all previous time steps, or to the entire published data from a previous time step.


\subsection{Trigger Actions}
Ascent executes a set of actions described by the user. 
%
These actions direct Ascent to create pipelines (i.e., transforming data), extracts (i.e., capture data), and  scenes (i.e., making pictures).
%
Trigger actions are defined by the same interface and thus all the same capabilities, including containing other triggers. 
%
Examples of trigger actions are saving the entire mesh to disk, creating a pipeline to transform the data and saving the resulting extract, or rendering images. 
%
Additionally, trigger actions can include other triggers that create a set of conditions(contained in multiple triggers) which all must  be satisfied before the final set of actions is performed.
%
%Initially, triggers are a sink in the data flow network. If the trigger fires, the sink is converted into an inner node of %the graph, and the data flow network is extended based on the actions provided  by the user. 
Since triggers have access to the input actions, it is possible for triggers to themselves modify the actions, creating possibilities for dynamically adaptive visualization workflows.
%

\iffalse
\subsection{Implementation}
Triggers functionality in Ascent are implemented in two classes.
%
The trigger base class has a number of responsibilities including providing access to input data, validating parameters (e.g., actions), calling a derived class's trigger function, and executing the actions if the trigger fires. 
%
Derived triggers initialize the base class to register what types of data require for its evaluation routine.
%
The only function a derived trigger is required to implement is the evaluation routine.
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