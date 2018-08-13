\section{Related Work}

Work by Ling et al.~\cite{ling_ldav} recognized the importance of adaptive triggers being generalizable across simulations, require no supervision, minimize communication overhead, and not have to keep previous time steps for analysis. 
%
And while their work achieves the goals previously mentioned, their method involves several machine learning algorithms that are computationally significant and therefore unfit for in situ analysis. 
%
Researchers Malakar et al.~\cite{malakar} take into account the overhead of in situ analysis and present a mathematical formulation for choosing the optimal output frequency given the system's constraints and test their analysis on molecular dynamics applications. 

There have been a number of works that have developed domain-specific triggers.
%
Work by Bennett et al.~\cite{bennett_siam} developed an application-specific trigger for ignition during combustion simulations. 
%
This work was then extended by Salloum et al.~\cite{Salloum_isav} who provided an alternative trigger metric that increased the robustness of the original work. 
%
Similarly, work by Zhao et al.~\cite{zhao_jc} and Ullrich et al.~\cite{ullrich_gmd} created domain-specific triggers for tropical cyclone trackers within climate simulations. 

Zhou and Chiang \cite{zhou_cgf} and Myers et al.~\cite{myers} utilized statistical methods to determine time steps of global and local significance, respectively, though the former work is not performed in situ.  
%
Banesh et al.~\cite{Banesh11} expanded on this work by utilizing change detection techniques for both time-based and specific parameter-based eddy identification in simulated ocean data. 
%
And while this research utilizes application-specific parameters for their detection, the concept of change point detection can be applied to other simulations \cite{wendelberger}. 

