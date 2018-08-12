\section{Introduction}

Computational simulations produce large amounts of data, 
and an important part of 
the workflow for these simulations is managing this  data.
%
Common actions available to the simulation for processing this data
are to save the data to disk,
to apply in situ visualization and analysis techniques,
or a combination of the two (i.e., use in situ techniques to transform and/or
subset data and then store the result to disk).
%
That said, perhaps the most common action for processing simulation data
is to discard it ---
the data produced at a given cycle is thrown away as the simulation advances.
%

A simulation's workflow for visualization, analysis, and/or storage (VAS) can be
either adaptive or non-adaptive.
%
With a non-adaptive workflow, the VAS actions performed happen based on some
fixed policy that is established before the simulation begins.
%
Examples of such fixed policies are to perform VAS every N cycles, to perform
VAS every time a simulation has advanced T seconds of simulation time,
or to perform VAS every M minutes of time the simulation runs on a computer.
%
With these workflows, much of the simulation's data is not inspected for
VAS purposes.
%
For example, if the policy is to save to disk every 100 cycles, then 
99\% of data is not subject to VAS, i.e., a temporal subsampling.
%
Temporal subsampling is often acceptable, as simulation data from one
cycle to the next is often very similar.
%
However, if the proportion of the data inspected gets too low,
then temporal subsamplings can be highly problematic, as key phenomena
may occur during the cycles not inspected.
%
In such cases, the effects of key phenomena may leave effects in subsequent
time slices that lead scientists to realize
that important information was lost during due to temporal subsampling,
in which case the simulation could be re-run.
%
Or, worse, the
phenomena may go undiscovered altogether.

Adaptive workflows work differently.
%
With adaptive workflows, the data produced by a simulation is 
regularly inspected (often every cycle).
%
The job of these inspections is to determine if interesting phenomena
has occurred.
%
If so, then the inspection process should prompt VAS actions to occur.
%
This approach has the potential to prevent the loss of key phenomena,
provided the right VAS actions are taken at the right time.
%
The approach has been gaining momentum with many recent
results (see the Related Work section), and is termed ``in situ triggers,''
since the inspection routines are applied  in situ to the simulation data
and the routines can ``trigger'' VAS actions to take place.

With this work, we describe the 
ASCENT in situ library's
mechnanism for in situ triggers.
%
Our focus with this work was providing a flexible system that would enable
a variety of trigger approaches.
%
Our system defines triggers as two orthogonal pieces:
(1) an inspection routine that determines when VAS should occur
and 
(2) the specific VAS actions to apply.
%
We feel this approach will encourage re-use, as ASCENT developers can
focus on developing many inspection routines and/or VAS actions, and then
pair them as appropriate.
%
In the remainder of this paper, we describe ASCENT's trigger system, as
well as the inspection routines and VAS actions we have developed to date.
%
The inspection routines are based on simulation field data, simulation mesh
topology, and performance data, and
our VAS actions involve all of the capabilities within ASCENT.


\if 0
ALPINE is a multi-institution effort funded by the U.S. Department of Energy's
Exascale Computing Project (ECP)~\cite{messina2017exascale}.
%
Its purpose is to deliver in situ infrastructure for visualization and
analysis to ECP application teams.
%
ALPINE's strategy is to support two existing in situ runtimes: 
VisIt's~\cite{VisIt} LibSim~\cite{LibSim} and ParaView's~\cite{ParaView} 
Catalyst~\cite{Catalyst}.
%
However, ALPINE has also developed a new in situ 
runtime, which is called ``ALPINE Ascent".
%which is also called ALPINE (and disambiguated as 
%``ALPINE in situ infrastructure" when discussed alongside the ALPINE project).
% \fix{Note: We want to call the infrastructure ``Ascent", to stem confusion.}

We believe ALPINE advances the state of the art in three distinct ways:
\begin{itemize}
\item \textbf{Support for modern supercomputing architectures.}  

ALPINE was designed with modern supercomputing architectures in mind.
%
It follows a hybrid parallel strategy, meaning it has support for both distributed-memory
parallelism across nodes and shared-memory parallelism within a node.
%
The shared-memory parallel support comes through usage of VTK-m~\cite{Moreland:CGA2016},
which encourages algorithm development using hardware-agnostic building blocks.
These building blocks
are replaced at compile time with efficient hardware-specific implementations, 
enabling portable performance over multiple architectures.
ALPINE's distributed-memory parallel support can come from either DIY~\cite{DIY} or MPI~\cite{MPI}.
%
ALPINE achieves this hybrid parallelism through use of a new library, called VTK-h (`h' for hybrid 
parallelism), that combines VTK-m and DIY/MPI.
VTK-h is  introduced later in this paper.
\item \textbf{Flyweight infrastructure.}
For ALPINE, the flyweight goal is realized in three ways: (1) an interface that
can easily be adopted by stakeholders, (2) minimal dependencies on other
software packages and small encumbrance on the binary size  of the simulation code,
and (3) minimal overheads incurred during processing, specifically with respect
to copying data and memory usage.
\item \textbf{Interoperability with software.}  
Although VTK-m plays a special role in the ALPINE project, the new Ascent runtime was designed to support additional
libraries.
%
Specifically Ascent makes use of a data flow library called ``Flow" to organize execution.
%
Flow is agnostic to the data models and libraries used in filters, and therefore can enable 
other libraries (such as R~\cite{team2000r}) to be used within Ascent.
%
Of course, it would be up to those libraries to provide support for parallelism, and additional
work would be needed to bridge between data models (for example, VTK-m to R or vice-versa).
\end{itemize}

%Contributions\fix{placeholder language}:
%\begin{itemize}
%\item First in situ library designed with a fully hybrid distributed-memory, shared-memory parallel architecture that runs portably on many-core architectures. 
%\item Flexible in situ framework that easily enables custom analysis (filters). \fix{weave filters from different runtimes together}
%\item Light weight integrations and a minimally expressive api (most operations have defaults).
%\end{itemize}

The paper is organized as follows:
Section~\ref{sec:interface} describes ALPINE's interface concepts, Section~\ref{sec:architecture}
describes the main components of ALPINE's infrastructure, and Section~\ref{sec:results} describes some initial results.

%related work

%staying tru to the requirements of strawman...

\fi
