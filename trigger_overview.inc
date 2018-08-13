\section{Trigger Infrastructure Overview}

\subsection{Trigger Actions}
Ascent executes a set of actions described by the user. 
%
These actions direct Ascent to create pipelines (i.e., transforming data), extracts (i.e., capture data), and  scenes (i.e., making pictures).
%
Trigger actions are defined by the same interface and thus all the same capabilities, including containing other triggers. 
%

\subsection{Trigger Input}
A triggers input by default is the entire data set published by the simulation to Ascent.
%
Optionally, triggers can consume the result of a pipeline, a set of data transformations. 
%
For example, the input to a trigger could be result of a clip and an iso-surface.


\subsection{Trigger Types}
The Ascent trigger infrastructure consists of different trigger types defined by the type of information used to evaluate the execution condition.
%
A trigger's type determines what class of information the triggers inspection routine will process.
%
The types of triggers supported in Ascent are:

\begin{itemize}
\item \textbf{Field}: execution based on information about a specific field on the mesh
\item \textbf{Mesh}: execution based on information about the mesh topology
\item \textbf{Performance}: execution based on performance information provided by the simulation
\fix{I am not sold on the term performance There is probably an overarching term that contains performance. Perhaps simulation state.}
\end{itemize}


Field triggers have access to all the values of a field on the mesh.
%
A simple example of a field trigger is one that fires when the maximum value exceeds a user defined threshold.
%
Mesh triggers are passed mesh topology data, and a example of mesh trigger would examine the mesh, firing when a tangle is detected.
%
Performance triggers are passed custom data published by the simulation.
%
\fix{Generally this data is performance information, but the simulation is free to publish any information.}
%
Unlike field and mesh triggers, performance trigger actions execute on a reduced version of the mesh, where each simulation domain is represented as a single cell. 

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

\subsection{Implementation}
Triggers in Ascent are implemented as a set of classes.
%
The trigger base class is responsible for executing the derived classes trigger method and executing the actions if the trigger fires. 
%
Each trigger type implements a derived type that marshals a subset data from the full input data, possibly the entire data set.
%
For example the field trigger class extracts the requested field from the data set and forwards it to the derived class.
%
Stateful triggers inherit from both the trigger base type and the Stateful class, which contains the facilities to save and retrieve state information. 
Figure~\ref{inheritance} shows the class hierarchy. 

\begin{figure}
\includegraphics[width=4cm]{images/inheritance}
\caption{\label{inheritance} An inheritance diagram for the Ascent trigger infrastructure of a custom mesh trigger.}
\end{figure}


