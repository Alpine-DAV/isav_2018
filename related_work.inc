\section{Related Work}
%\fix{I love this characterization, but I wonder if it distracts ...
%we don't really circle back to this point, and it isn't related to our
%own contribution.}
%Ideally, a trigger is adaptive and generalizable across simulations. 
%
%They should require no supervision, should minimize both communication and analysis overheads, and not have to keep previous time steps for analysis. 
%
%But in some cases, not all of these trigger characteristics can be met. 
%

Recent research into triggers can be characterized into two categories: 
triggers that are domain-agnostic and triggers that are domain-specific, and the organization of this section mirrors this current division in research. 
%
Conceptually, each of the works surveyed below could be supported by our
system, although in some cases new inspection routines or VAS actions would
need to be added to Ascent.

\subsection{Domain-Agnostic Triggers}
Domain-agnostic triggers leverage algorithms that apply broadly to arrays of values (e.g., simple aggregation, summary statistics, many machine learning techniques). These can be used generically by any simulation application, so there is value to sharing implementations.

Work by Ling et al.~\cite{ling_ldav} implements an autonomous, domain-agnostic trigger, but their method involves several machine learning algorithms that are computationally significant, which is likely a barrier for many 
in situ use cases. 
%
Malakar et al.~\cite{malakar} take into account the computational strain of in situ analysis and present a mathematical formula for choosing the optimal process-to-node mapping given the system's constraints.  
%\fix{Hank:why is this reference grouped with the other one?  What do they have in common?  (I don't see it.)}
%\fix{Nicole: It plays off the computational intensity shown in Ling's work and that consideration needs to be taken when applying in situ algorithms. Hank: but what about the rest of the section?  Are Zhou/Chiang, Myers, etc. not even in consideration?  My comment then expands to the organization of the whole subsection.}

Zhou and Chiang \cite{zhou_cgf} and Myers et al.~\cite{myers} utilized statistical methods to determine time steps of global and local significance, respectively, though the former work is not performed in situ.  
%
Banesh et al.~\cite{banesh} expanded on this work by utilizing change detection techniques for both time-based and specific parameter-based eddy identification in simulated ocean data. 
%
And while this research utilizes application-specific parameters for their detection, the concept of change point detection can be applied to other simulations \cite{wendelberger}. 

\subsection{Domain-Specific Triggers}
Domain-specific triggers leverage algorithms that use special knowledge of either a simulation method or its scientific domain 
(e.g., triggering on a gradient calculated using specific finite-element method,
or enstrophy for as a measure of dissipation). These are not as broadly applicable to any simulation, so it is important for an in situ infrastructure to support integrating custom algorithms to support these types of triggers.

A number of works have developed domain-specific triggers.
%There have been a number of works that have developed domain-specific triggers.
%
Work by Bennett et al.~\cite{bennett_siam} developed an application-specific trigger for ignition during combustion simulations. 
%
This work was then extended by Salloum et al.~\cite{Salloum_isav}, who provided an alternative trigger metric that increased the robustness of the original work. 
%
Similarly, work by Zhao et al.~\cite{zhao_jc} and Ullrich et al.~\cite{ullrich_gmd} created domain-specific triggers for tropical cyclone trackers within climate simulations.
%\fix{should we elaborate more? (drive home the point that these looked a physics specific to the domain, etc?)}


