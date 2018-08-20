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
The trigger implementation in Ascent is simply another filter in a larger data-flow network.

\subsection{Trigger Input}
A trigger's input by default is the simulation's entire data set.
%
Optionally, triggers can consume the result of a set of data transformations. 
%
For example, the input of a trigger could be result of a clip and an iso-surface.
%

\subsection{Trigger Types}
Triggers can be broadly categorized by the types of data used inside the evaluation routine and whether or not the trigger maintains state.
%
%The Ascent trigger infrastructure supports several different trigger types defined by the type of information used to evaluate the execution condition.
%
%A trigger's type determines what class of information the triggers inspection routine need to access.
%
The trigger types supported in Ascent are:

\begin{itemize}
\item \textbf{Field}: execution based on information about a specific field on the mesh
\item \textbf{Mesh}: execution based on information about the mesh topology
\item \textbf{State}: execution based on state information provided by the simulation
\end{itemize}

Field triggers have access to all the values of a field on the mesh.
%
A simple example of a field trigger is one that fires when the maximum value exceeds a user defined threshold.
%
Mesh triggers are passed mesh topology data, and a example of mesh trigger would examine the mesh, firing when a tangle is detected.
%
State triggers are passed custom data published by the simulation.
%
State data can not only include basic information like cycle and time, it can include simulation performance metrics.  
%
Examples of state triggers are fire every $X$ cycles or fire when a performance metric rises above some critical threshold.
%

Our trigger infrastructure supports combining types, enabling the creation more complex inspection routines.
%nlike field and mesh triggers, state trigger actions execute have the option of using a reduced version of the mesh, where each simulation domain is represented as a single cell. 
%
%By using a reduced version of the mesh, performance data can be projected back onto the simulation mesh, providing valuable insights into performance problems.

In addition to trigger type, the Ascent trigger infrastructure allows two trigger variations:
\begin{itemize}
\item \textbf{Stateless}: trigger execution is independent of any previous invocations
\item \textbf{Stateful}: the trigger that maintains state information about previous invocations
\end{itemize}
%
By default a trigger is stateless, i.e. a trigger's inspection routine has no knowledge of any previous invocations.
%
The execution of a stateless trigger is completely dependent on the information in the current time step.
%
Stateful triggers maintains state information from past executions.
%
For example, a trigger might only fire if the information in the current time step varies significantly from the last time step.
%
The state data is completely arbitrary and could range from a single value, a series of values from all previous time steps, or to the entire published data from a previous time step.
%

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
