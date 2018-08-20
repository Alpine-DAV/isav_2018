\section{Trigger Infrastructure Overview}
\fix{HIGHLIGHT Python}
Our trigger infrastructure is implemented in Ascent~\cite{Larsen:2017:ASI:3144769.3144778}, a fly-weight in situ visualization and analysis infrastructure.
%
Ascent's flexibility is derived from a generic data flow network called Flow.
%\fix{Is there a reference for Flow?  Was it described in the ISAV17 paper?  If not, I propose that it go unnamed.}
%\fix{Matt: yes it was.}
%
Filters in Flow have arbitrary inputs and outputs, and each component of Ascent is implemented as a Flow filter.
%
The Ascent runtime translates data and actions described in the high level API into a data flow network that Flow executes.
%
Using this modular architecture, all of Ascent's components can be connected together.
%
The trigger implementation in Ascent is simply another filter in a larger data flow network.


\subsection{Trigger Input}
A trigger's input by default is the simulation's entire data set.
%
Optionally, triggers can consume the result of a set of data transformations. 
%
For example, the input of a trigger could be result of a clip and an iso-surface.
%
%\fix{I think the preceding sentence involves more understanding of the Ascent paradigm than the reader may have.  I propose:
%}
%\fix{Confused.  I believe it would read better to me if you added an i.e., as
%in ``pipeline, i.e., a set of...''}
%
\fix{Ascent triggers make decisions using Mesh-Blueprint data. 
In addition to the fact that results from pipelines can be feed in (so we can reuse VAS actions) - the blueprint angle is important high-level point -- we can pull from next section}


\subsection{Trigger Types}
\fix{Cyrus's Note: Some of this I have captured in the Trigger Decisions Section, some of it can be moved into Trigger Input section above}
The Ascent trigger infrastructure consists of different trigger types defined by the type of information used to evaluate the execution condition.
%
A trigger's type determines what class of information the triggers inspection routine will process.
%
The types of triggers supported in Ascent are:

\begin{itemize}
\item \textbf{Field}: execution based on information about a specific field on the mesh
\item \textbf{Mesh}: execution based on information about the mesh topology
\item \textbf{State}: execution based on state information provided by the simulation
%\fix{I am not sold on the term performance There is probably an overarching term that contains performance. Perhaps simulation state.}
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
%\fix{Generally this data is performance information, but the simulation is free to publish any information.}
%
Examples of state triggers are fire every $X$ cycles or fire when a performance metric rises above some critical threshold.
Unlike field and mesh triggers, state trigger actions execute have the option of using a reduced version of the mesh, where each simulation domain is represented as a single cell. 
%
By using a reduced version of the mesh, performance data can be projected back onto the simulation mesh, providing valuable insights into performance problems.
%\fix{This seems very specific.  Would something like looking at wallclock and triggering every ten minutes fit under performance trigger?  ... I note sim code do this %already ... here, I am suggesting they do it through Ascent.}

In addition to specific types of triggers, the Ascent trigger infrastructure allows two trigger variations:
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

\subsection{Trigger Decisions}

Two Parts: Data Reduction and Decide to Fire.

\subsubsection{Reduce the data}
%Triggers need to be cheap. They need to be useful. 

The first step in the decision process is to reduce the input simulation data. The data reduction may leverage a pipeline of existing VAS actions, or it may be a simple data summarization algorithm. While triggers can leverage expensive algorithms, in practice they are often used to reduce overall computation by screening data before applying more expensive VAS actions. 
Because of this, inexpensive data reduction techniques are important. 


%From a user's perspective there are a few common constructs that are both cheap and useful. 
The most basic technique is a simple aggregation of a field or a topological feature (minimum, maximum, mean, variance, integral, etc).
These are inexpensive because they do not require much intermediate memory, are typically  O(number\_of\_elements) and their distributed-memory implementations are straightforward and scalable.


A more sophisticated way to reduce data is to calculate a coarse distribution of a field or a topological feature. This can be calculated inexpensively with a counting or weight-based histogram. (mention: exact or cumulative distribution would require a sort, which is not cheap). Multi dimensional binning can also be used to calculate coarse averages which can be overlaid back onto the mesh topology as input to create more complex derived quantities for summarization. \cite{ecf07} For distributions, you can further reduce them using another summary metric, such as identifying percentiles or calculating the shannon entropy. 

\subsubsection{Decide to fire}

The next step in the decision process is to apply a final test to determine if the trigger should fire. 

You can base the decision on the simulation's current aggregate value, or can you can make a decision using the time history of the aggregate values from previously published data. 

The standard menu of numeric comparison operations (e.g. greater-than, less-than, equal, etc) are useful building blocks for deciding to fire using a single value. 
 
For the time history, you may care about critical points of the curve (minimum, maximum, saddle points),  or about a more complex property, such as the value changing more than some percentage of the current peak. You may need a smoothing function to make robust decisions about a time history curve. 



\subsection{Trigger Actions}
Ascent executes a set of actions described by the user. 
%
These actions direct Ascent to create pipelines (i.e., transforming data), extracts (i.e., capture data), and  scenes (i.e., making pictures).
%
Trigger actions are defined by the same interface and thus all the same capabilities, including containing other triggers. 
%
Some examples of trigger actions are saving the entire mesh to disk, creating a pipeline to transform the data and saving the resulting extract, or rendering images. 
%
Additionally, trigger actions can include other triggers that create a set of conditions(contained in multiple triggers) which all must  be satisfied before the final set of actions is performed.
%
Initially, triggers are a sink in the data flow network. If the trigger fires, the sink is converted into an inner node of the graph, and the data flow network is extended based on the actions provided  by the user. Since triggers have access to the input actions, it is possible for triggers to themselves modify the actions, creating possibilities for dynamically adaptive visualization workflows.
%

\subsection{Implementation}
Triggers in Ascent are implemented as a set of classes.
%
The trigger base class is responsible for executing the derived classes trigger method and executing the actions if the trigger fires. 
%
Figure~\ref{inheritance} shows the class hierarchy of the various trigger types.
%
Derived trigger types implement a ``get\_data'' method that marshals a subset data from the full input data, possibly the entire data set.
%
For example the field trigger class extracts the requested field from the data set and forwards it to the derived class.
%
Stateful triggers inherit from both the trigger base type and the Stateful class, which contains the facilities to save and retrieve state information. 

\fix{Triggers can be defined in the host code (domain-specific), directly into ascent (domain-agnostic), or exist as a python script on the file system (both). That said, one of the goals of the trigger infrastructure design is to encourage the sharing and re-use of different triggers. }

\begin{figure}
\includegraphics[width=6cm]{images/inheritance}
\caption{\label{inheritance} An inheritance diagram for the Ascent trigger infrastructure of a custom mesh trigger.}
\end{figure}

\begin{lstlisting}[caption={The trigger base class API. Each derived trigger type implements the ``get\_data'' method that retreives the appopriate topology, field, or state data from the input.},captionpos=b]
class Trigger : public Filter
{
public:
  virtual bool trigger(const conduit::Node& data) = 0;
  void exectute()
  {
  	bool fire = trigger(get_data());
  	if(fire)
  	{
  	  // execute actions
  	} 
  }
protected:
  virtual const conduit::Node& get_data() = 0;
};
//Alternative:
class Trigger : public ascent::flow::Filter
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
};

class MeshTrigger : public Trigger
{
protected:
  const conduit::Node& get_mesh();
};
\end{lstlisting}



\begin{lstlisting}[caption={The trigger state base class that provides methods for setting and retrieving state inforation stored in conduit nodes. State data is stored in a static key-value store accessible to all trigggers },captionpos=b]
class TriggerState
{
protected:
 conduit::Node& get_state(std::string key);
 void set_state(std::string key, conduit::Node &state);
};

\end{lstlisting}
