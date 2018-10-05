\section{Conclusion}
In this paper, we described a flexible system for in situ triggers inside of Ascent.
%
%Trigger implementations with traditional visualization tools must be written outside of the tool and have to manually implement actions.
Supporting trigger use cases with traditional visualization tools requires control flow logic be written outside of the tool to coordinate trigger decisions and VAS actions. 
%
In contrast, our work directly integrates trigger capabilities into an in situ visualization and analysis infrastructure to simplify these use cases.
%
The trigger system uses modular components that provide different options for VAS actions, comparisons, and reduction methods (if needed).
%
For trigger developers, we provide re-usable and interchangeable components that lower barriers to trigger development.
%
We also demonstrated how different categories of triggers can be used to identify features of interest (e.g., threshold), filter out un-needed data (e.g., maximum radial distance). and highlight simulation performance bottlenecks (e.g., simulation state).

For future work, we intend to continue to refine Ascent's in situ trigger system and provide additional reduction and summarization building blocks.
%
Additionally, we would like to explore more dynamic triggers workflows where trigger modify the input actions.
 


