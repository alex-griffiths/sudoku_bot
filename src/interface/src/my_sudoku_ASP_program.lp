% define the size of the subgrid. The size of the subgrid is the square root of the overall board size.
#const sg = 5.

row(1..sg*sg).
col(1..sg*sg).
n(1..sg*sg).

% for every gridNum(X,Y,N) choose one N such that the row and col condition is satisfied. Basically, fill out all the possible number positions
{gridNum(X, Y, N): n(N)}=1 :- row(X), col(Y).

% constain number to once per row/col
:- gridNum(X,Y,N), gridNum(A,Y,N), X!=A.
:- gridNum(X,Y,N), gridNum(X,B,N), Y!=B.

% Define what a subgrid is, and where each one should begin/end
subgrid(X,Y,A,B) :- row(X), row(A), col(Y), col(B), (X-1)/sg == (A-1)/sg, (Y-1)/sg == (B-1)/sg.

% Constrain number to only show once per subgrid
:- gridNum(X,Y,V), gridNum(A,B,V), subgrid(X,Y,A,B), X!=A, Y!=B.
