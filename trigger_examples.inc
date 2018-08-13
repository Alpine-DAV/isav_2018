\section{Trigger Examples}
Examples of triggers

\subsection{Performance Trigger Example}
Kripke~\cite{kunen2015kripke} is a deterministic neutron transport physics proxy for ARDRA, a code at Lawrence Livermore National Laboratory.
%
Kripke is included with Ascent and implements a parallel sweep solver.
%
Each sweep consists of wavefronts for each quadrature direction that propagate through the MPI tasks.
%
The Ascent integration captures sweep solver performance metrics, including the maximum backlog of incoming requests from different quadrature directions.
%

In order to show an overall view of the maximum backlog for each MPI task, we created a performance trigger on the maximum back that fires when the maximum backlog exceeds 4 unanswered requests.
%
Using a volume plot shown in figure~\ref{max_backlog}, tasks with higher backlogs appear more opaque than the other.
%
Using performance visualization techniques can both highlight performance bottlenecks and provide useful visual debugging information for simulation developers.

\begin{figure}
\includegraphics[width=8cm]{images/kripke_max_backlog}
\caption{\label{max_backlog} A volume plot of the each MPI task's maximum backlog of unanswered queries from surrounding tasks from a run consisting of 512 MPI tasks.}
\end{figure}