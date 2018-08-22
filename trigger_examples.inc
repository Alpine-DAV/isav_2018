\section{Trigger Examples}
\begin{figure*}[h]

\centering
\includegraphics[width=.24\textwidth]{images/max_radial}\hfill
\includegraphics[width=.24\textwidth]{images/kripke_max_backlog}\hfill
\includegraphics[width=.24\textwidth]{images/clover_plot}
\includegraphics[width=.24\textwidth]{images/clover_energy}

\caption{\label{images_figure}default}
\end{figure*}


In this section, we demonstrate a few examples of using Ascent triggers that inspect different combinations of trigger inputs.
These examples use Ascent's built-in proxy applications, because they provide a simple path to motivate and show common use cases.
%


\subsection{Field Trigger Example}
The maximum value of a field value over time is a quantity of interest in physics simulations.
%
Figure~\ref{images_figure}


\subsection{Stateful Field Trigger Example}
\iffalse
A simple example of a stateful trigger is a trigger based on the shannon entropy of a field's distribution function.
%
The entropy trigger calculates the distributed-memory histogram of a field on the data set, and uses the values of the histogram to calculate entropy using the formula:
\begin{equation}
H(X) = -\sum\limits_{i=1}^n P(x_{i}) log_{2}P(x_{i})
\end{equation}
where $x_{i}$ is the value of histogram bin.
%
Entropy is calculated at every cycle and stored into the state of the trigger.
%
Whenever entropy differs from the previous cycle by a user defined threshold (e.g., 20.0\%), the trigger fires, executing the trigger actions.

Figure~\ref{clover_entropy} plots the entropy value of energy over 200 cycles during a run of Cloverleaf3D, a hydrodynamics proxy-application included with Ascent.
%
\fix{Initially, entropy values vary significantly and ...}

\begin{figure}
\includegraphics[width=8cm]{images/clover_entropy}
\caption{\label{clover_entropy} A plot of entropy over cycles of the energy field during a run of the hydrodynamics proxy-application Cloverleaf3D.}
\end{figure}

\fi

%\begin{figure}
%\includegraphics[width=8cm]{images/clover_energy}
%\caption{\label{clover_entropy} A plot of energy over cycles of the energy field during a run of the hydrodynamics proxy-application Cloverleaf3D.}
%\end{figure}

\subsection{Maximum Radial Distance Example}
There are several cases where expensive processing is only necessary after a simulation model has evolved to a certain regime that cannot easily be identified a priori based on simulation time. 

To demonstrate this type of use case, we developed a trigger for Lulesh~\cite{karlin2013lulesh}, 
%included with Ascent, is 
a shock-hydrodynamics proxy-application that models the evolution of a shock-wave as it propagates from the origin of the simulation.
%
The trigger executes actions after the main shock-wave has reached a specified radial distance from the origin of the simulation.

%\begin{figure}
%\includegraphics[width=5cm]{images/max_radial}
%\caption{\label{clover_entropy} A plot of energy over cycles of the energy field during a run of the hydrodynamics proxy-application Cloverleaf3D.}
%\end{figure}
%Often times scientists are only interested in analyzing simulation once the shock-wave has reached some radial distance from the origin of the wave.
%
Knowing exactly when the shock-wave passes a point is problem dependent since it changes both with the input deck and problem size. 
%
In Ascent the maximum radial distance trigger combines field, topology, and coordinate set information to track the distance of the element containing the maximum pressure value.
%
When this distance exceeds the threshold, the trigger fires.


\subsection{Simulation State Trigger Example}
Kripke~\cite{kunen2015kripke} is a physics proxy-application for ARDRA, a code at Lawrence Livermore National Laboratory.
%
Kripke is included with Ascent and implements a parallel sweep solver to solve the equations for deterministic neutron transport.
%
Each sweep consists of wavefronts for each quadrature direction that propagate through the MPI tasks.
%
The Ascent integration captures sweep solver performance metrics, including the maximum backlog of incoming requests from different quadrature directions.
%

In order to show an overall view of the maximum backlog for each MPI task, we created a performance trigger on the maximum back that fires when the maximum backlog exceeds 4 unanswered requests.
%
Using a volume plot shown in figure~\ref{max_backlog}, MPI tasks with higher backlogs appear more opaque than tasks with lower backlogs.
%
Using performance visualization techniques can both highlight performance bottlenecks and provide useful visual debugging information for simulation developers.

%\begin{figure}
%\includegraphics[width=8cm]{images/kripke_max_backlog}
%\caption{\label{max_backlog} A volume plot of the each MPI task's maximum backlog of unanswered queries from surrounding tasks from a run consisting of 512 MPI %tasks.}
%\end{figure}

\subsection{Dynamic Actions Example}
Simple example of dynamic actions.