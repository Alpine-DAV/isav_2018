\section{Related Work}
\fix{I love this characterization, but I wonder if it distracts ...
we don't really circle back to this point, and it isn't related to our
own contribution.}
Ideally, a trigger is adaptive and generalizable across simulations. 
%
They should require no supervision, should minimize both communication and analysis overheads, and not have to keep previous time steps for analysis. 
%
But in some cases, not all of these trigger characteristics can be met. 
%
The recent research into triggers can be divided into two camps: triggers that are domain-specific and triggers that are domain-agnostic. 
%
The organization of this section mirrors this current division in research. 

\fix{Hank proposes this intro paragraph:}
Recent research into triggers can be characterized into two categories: 
triggers that are domain-specific and triggers that are domain-agnostic. 
%
The organization of this section mirrors this current division in research. 

\subsection{Domain-Agnostic Triggers}

Work by Ling et al.~\cite{ling_ldav} implements an autonomous, domain-agnostic trigger, but their method involves several machine learning algorithms that are computationally significant \fix{Hank alternate: which is a barrier for many 
in situ use cases.} and therefore unfit for in situ analysis. 
%
Researchers \fix{Why include  the word ``researchers''?  Why does this one reference necessitate different handling in terms of how the reference is referred to??} Malakar et al.~\cite{malakar} take into account the overhead of in situ analysis and present a mathematical formulation for choosing the optimal output frequency given the system's constraints and test their analysis on molecular dynamics applications.  \fix{why is this reference grouped with the other one?  What do they have in common?  (I don't see it.)}

Zhou and Chiang \cite{zhou_cgf} and Myers et al.~\cite{myers} utilized statistical methods to determine time steps of global and local significance, respectively, though the former work is not performed in situ.  
%
Banesh et al.~\cite{Banesh11} expanded on this work by utilizing change detection techniques for both time-based and specific parameter-based eddy identification in simulated ocean data. 
%
And while this research utilizes application-specific parameters for their detection, the concept of change point detection can be applied to other simulations \cite{wendelberger}. 

\subsection{Domain-Specific Triggers}
There have been a number of works that have developed domain-specific triggers.
%
Work by Bennett et al.~\cite{bennett_siam} developed an application-specific trigger for ignition during combustion simulations. 
%
This work was then extended by Salloum et al.~\cite{Salloum_isav} who provided an alternative trigger metric that increased the robustness of the original work. 
%
Similarly, work by Zhao et al.~\cite{zhao_jc} and Ullrich et al.~\cite{ullrich_gmd} created domain-specific triggers for tropical cyclone trackers within climate simulations. 


