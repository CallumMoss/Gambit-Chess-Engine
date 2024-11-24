24/11/2024:
Added checks for 3 fold repetition and 50 move rule

23/11/2024:
Implemented iterative deepening

22/11/2024:
Reorganised to relevant files, fixed some bugs and worked on basics for search

21/11/2024:
Ran SPRTs for negamax_fixed_depth_2 vs random_mover and negamax_iterative_deepening vs negamax_fixed_depth_2. Found issues so implemented quintescence search

19/11/2024 - 20/11/2024
Working on UCI, v1 complete

16/11/2024 - 18/11/2024
Birthday Celebrations

14/11/2024 - 15/11/2024
Working on UCI

13/11/2024
Refactored until passes perft 6 for starting position.

11/11/2024 - 12/11/2024
Lots of work. Fixing various move gen issues and testing with perft.

09/11/2024 - 10/11/2024
Major Revisions: Updated things to work with Moves instead of u64s. Made a search and eval function with copy make move

07/11/2024 - 08/11/2024
N/A

06/11/2024
Research into implementation of alpha beta and negamax

05/11/2024
Started work on search and eval.

03/11/2024 - 04/11/2024
Working on machine learning assignment

02/11/2024
Refactored parameters for methods in position class. Looked into doxygen for proper commenting as aim to clean up code before moving forwards.

01/11/2024
Meeting with Eduard to discuss project progress

30/10/2024 - 31/10/2024
N/A

29/10/2024
Began thinking about copy-make, legality checks and en-passant. Could check any blockers for the king, then work from there.
Or could get attack mask post blockers and & with king. Would simply use the magic gen but not split the moves into many.

26/10/2024 - 29/10/2024
Doing assignments and more interviews

25/10/2024
Added protocol for if there is an enpassant. Checking if en passant is caused to be done.

18/10/2024 - 24/10/2024
Applying to jobs and doing interviews

17/10/2024
Used current magic techniques to generate all moves for all sliding pieces on all squares. En passant, castling and promotions to be done.

16/10/2024
Worked on generating move tables for sliding pieces using precomputed magics. Can now generate rook moves for all positions.

14/10/2024 - 15/10/2024
More work on magics.
Managed to generate magic numbers and corresponding tables

13/10/2024
Organised some code, ammended some errors on magics

12/10/2024
Continued work on magics, should be done tomorrow, made some notes in daybook about what to do.

11/10/2024
Added piece values and some comments

05/10/2024 - 10/10/2024
Planning thoroughly for self and project plan
Writing up project plan
Adding notes to daybook for later reference that didnt fit in the project plan

04/10/2024
Meeting with Eduard to discuss the project, talking about where we want to go with the project and whether it is feasible

01/10/2024 - 03/10/2024
Planning and further research conducted

30/09/2024
Pushed work done over the summer (Board representation and move gen for knights, kings and some pawn moves, as well as progress made on sliders)