class Board {
private:
    // data
    int grid[8][8];
    Texture2D textures[27];
    int squareSize, startX, startY;
 
    // drag state
    bool isMousePressed;
    int takeRow, takeCol;   // square the piece was picked up from (-1 = nothing picked)
    int turn = 2;

    bool promoting = false;
    int promoRow = -1;
    int promoCol = -1;
    int promoColor = 0;
    int promoStart = 0;
 
    // helpers used only inside the class
    void initGrid();
    void loadTextures();
    bool isInsideBoard(int x, int y) const;
    bool pawnRule(int row, int col, int color) const;
    bool knightRule(int row, int col, int color) const;
    bool bishopRule(int row, int col, int color) const;
    bool rookRule(int row, int col, int color) const;
    bool queenRule(int row, int col, int color) const;
    bool kingRule(int row, int col, int color) const;
    bool isCheck(int color);
    bool checkMate(int color);
 
public:

    bool checkmate = false;
    bool stalemate = false;

    Board(int startX, int startY, int squareSize);
    ~Board();
 
    void setTurn(int pieceColor);
    void draw() const;           // draws the squares
    void handleInput();          // mouse pick up / drop logic
    void drawPieces() const;     // draws the pieces (and the one being dragged)
    void drawDebugInfo() const;  // your X, Y, col, row text
    bool isValidMove(int row, int col) const;
};