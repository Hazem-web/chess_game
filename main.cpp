#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
using namespace sf;
using namespace std;

class Move;
class Board;

enum ChessColor{
    None,
    White,
    Black
};

enum ChessPiece{
    NONE,
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};

enum MoveType{
    Normal,
    CastelKS,
    CastelQS,
    DoublePawn,
    Promotion
};

enum EndReason{
    CheckMate,
    StaleMate
};

class PlayerExtensions{
    public:
        ChessColor static opponent(ChessColor i){
            switch (i){
                case ChessColor::White:
                    return ChessColor::Black;
                    break;
                case ChessColor::Black:
                    return ChessColor::White;
                    break;
                default:
                    return ChessColor::None;
                    break;
            }
        }
};

class Direction{
    private:
        int rowDelta;
        int columnDelta;
        Direction static North;
        Direction static South;
        Direction static East;
        Direction static West;
        Direction static NorthEast;
        Direction static NorthWest;
        Direction static SouthEast;
        Direction static SouthWest;
    public:
        Direction(int r,int c){
            rowDelta=r;
            columnDelta=c;
        }
        void setRowDelta(int r){
            rowDelta=r;
        }
        void setColumnDelta(int c){
            columnDelta=c;
        }
        int getRowDelta(){
            return rowDelta;
        }
        int getColumnDelta(){
            return columnDelta;
        }
        Direction static getNorth(){
            return North;
        }
        Direction static getSouth(){
            return South;
        }
        Direction static getEast(){
            return East;
        }
        Direction static getWest(){
            return West;
        }
        Direction static getNorthEast(){
            return NorthEast;
        }
        Direction static getSouthEast(){
            return SouthEast;
        }
        Direction static getNorthWest(){
            return NorthWest;
        }
        Direction static getSouthWest(){
            return SouthWest;
        }
        Direction operator+(Direction d){
            return Direction(rowDelta+d.rowDelta,columnDelta+d.columnDelta);
        }
        Direction operator*(int scalar){
            return Direction(rowDelta*scalar,columnDelta*scalar);
        }
};
Direction Direction::North=Direction(-1,0);
Direction Direction::South=Direction(1,0);
Direction Direction::East=Direction(0,1);
Direction Direction::West=Direction(0,-1);
Direction Direction::NorthEast=Direction::getNorth()+Direction::getEast();
Direction Direction::NorthWest=Direction::getNorth()+Direction::getWest();
Direction Direction::SouthEast=Direction::getSouth()+Direction::getEast();
Direction Direction::SouthWest=Direction::getSouth()+Direction::getWest();

class Position{
    private:
        int row;
        int column;
    public:
        Position(int r,int c){
            row=r;
            column=c;
        }
        void setRow(int r){
            row=r;
        }
        void setColumn(int c){
            column=c;
        }
        int getRow(){
            return row;
        }
        int getColumn(){
            return column;
        }
        ChessColor squareColor(){
            if((row+column)%2==0)
                return ChessColor::White;
            return ChessColor::Black;
        }
        bool operator==(Position p){
            if(row==p.row && column==p.column)
                return true;
            return false;
        }
        bool operator!=(Position p){
            if(row==p.row && column==p.column)
                return false;
            return true;
        }
        Position operator+(Direction d){
            return Position(row+d.getRowDelta(),column+d.getColumnDelta());
        }

};

class Peice{
    protected:
        ChessPiece type;
        ChessColor color;
        bool hasMoved=false;
        vector<Position> movePosition(Position from,Board &board,Direction dir);
        vector<Position> movePositions(Position from,Board &board,Direction dir[],int s);
    public:
        Peice(){
            type=ChessPiece::NONE;
            color=ChessColor::None;
        }
        void setHasMoved(bool h){
            hasMoved=h;
        }
        ChessPiece getType(){
            return type;
        }
        ChessColor getColor(){
            return color;
        }
        bool getHasMoved(){
            return hasMoved;
        }
        virtual Peice* copy();
        virtual vector<Move*> getMoves(Position from,Board &board);
        bool operator==(Peice p){
            if(p.type==type && p.color==color)
                return true;
            return false;
        }
        virtual bool canCaptureOpponentKing(Position from,Board &board);
};

class Pawn: public Peice{
    private:
        Direction forwardMove;
        static bool canMoveto(Position pos,Board &board);
        bool canCaputreAt(Position pos,Board &board);
        vector<Move*> forwardMoves(Position pos,Board &board);
        vector<Move*> diagonalMoves(Position pos,Board &board);
        static vector<Move*> promotionMoves(Position from,Position to);
    public:
        Pawn(ChessColor c):forwardMove(0,0){
            type=ChessPiece::PAWN;
            color=c;
            if(c==ChessColor::White)
                forwardMove=Direction::getNorth();
            else
                forwardMove=Direction::getSouth();
        }
        Pawn* copy(){
            Pawn *p=new Pawn(color);
            p->hasMoved=hasMoved;
            return p;
        }
        vector<Move*> getMoves(Position from,Board &board);
        bool canCaptureOpponentKing(Position from,Board &board);
};

class Bishop:public Peice{
    private:
        static Direction dirs[4];
    public:
        Bishop(ChessColor c){
            type=ChessPiece::BISHOP;
            color=c;
        }
        Bishop* copy(){
            Bishop *b=new Bishop(color);
            b->hasMoved=hasMoved;
            return b;
        }
        vector<Move*> getMoves(Position from,Board &board);
};

Direction Bishop::dirs[4]={Direction::getNorthWest(),Direction::getNorthEast(),Direction::getSouthEast(),Direction::getSouthWest()};

class Knight:public Peice{
    private:
        static vector<Position>  potentialToPosition(Position from){
            Direction verticalDirs[2]={Direction::getNorth(),Direction::getSouth()};
            Direction horizontalDirs[2]={Direction::getEast(),Direction::getWest()};
            vector<Position> totalPos;
            for(int i=0;i<2;i++){
                for(int j=0;j<2;j++){
                    totalPos.push_back((from+(verticalDirs[i]*2))+horizontalDirs[j]);
                    totalPos.push_back((from+(horizontalDirs[j]*2))+verticalDirs[i]);
                }
            }
            return totalPos;
        }
        vector<Position> availablePositions(Position from,Board &board);
    public:
        Knight(ChessColor c){
            type=ChessPiece::KNIGHT;
            color=c;
        }
        Knight* copy(){
            Knight *k=new Knight(color);
            k->hasMoved=hasMoved;
            return k;
        }
        vector<Move*> getMoves(Position from,Board &board);
};

class Rook:public Peice{
    private:
        static Direction dirs[4];
    public:
        Rook(ChessColor c){
            type=ChessPiece::ROOK;
            color=c;
        }
        Rook* copy(){
            Rook *r=new Rook(color);
            r->hasMoved=hasMoved;
            return r;
        }
        vector<Move*> getMoves(Position from,Board &board);
};

Direction Rook::dirs[4]={Direction::getNorth(),Direction::getSouth(),Direction::getEast(),Direction::getWest()};

class Queen:public Peice{
    private:
        static Direction dirs[8];
    public:
        Queen(ChessColor c){
            type=ChessPiece::QUEEN;
            color=c;
        }
        Queen* copy(){
            Queen *q=new Queen(color);
            q->hasMoved=hasMoved;
            return q;
        }
        vector<Move*> getMoves(Position from,Board &board);
};

Direction Queen::dirs[8]={Direction::getNorth(),Direction::getSouth(),Direction::getEast(),Direction::getWest()
,Direction::getNorthWest(),Direction::getNorthEast(),Direction::getSouthEast(),Direction::getSouthWest()};

class King:public Peice{
    private:
        static Direction dirs[8];
        vector<Position> availablePositions(Position from,Board &board);
        static bool isUnmovedRook(Position pos,Board &board);
        static bool allEmpty(vector<Position> positions,Board &board);
        bool canCastleKingSide(Position from,Board &board);
        bool canCastleQueenSide(Position from,Board &board);
    public:
        King(ChessColor c){
            type=ChessPiece::KING;
            color=c;
        }
        King* copy(){
            King *k=new King(color);
            k->hasMoved=hasMoved;
            return k;
        }
        vector<Move*> getMoves(Position from,Board &board);
        bool canCaptureOpponentKing(Position from,Board &board);
};
Direction King::dirs[8]={Direction::getNorth(),Direction::getSouth(),Direction::getEast(),Direction::getWest()
,Direction::getNorthWest(),Direction::getNorthEast(),Direction::getSouthEast(),Direction::getSouthWest()};

class Board{
    private:
        Peice *pieces[8][8];
        void addStartPieces(){
            pieces[0][0]=new Rook(ChessColor::Black);
            pieces[0][1]=new Knight(ChessColor::Black);
            pieces[0][2]=new Bishop(ChessColor::Black);
            pieces[0][3]=new Queen(ChessColor::Black);
            pieces[0][4]=new King(ChessColor::Black);
            pieces[0][5]=new Bishop(ChessColor::Black);
            pieces[0][6]=new Knight(ChessColor::Black);
            pieces[0][7]=new Rook(ChessColor::Black);
            for(int i=0;i<8;i++)
                pieces[1][i]=new Pawn(ChessColor::Black);

            pieces[7][0]=new Rook(ChessColor::White);
            pieces[7][1]=new Knight(ChessColor::White);
            pieces[7][2]=new Bishop(ChessColor::White);
            pieces[7][3]=new Queen(ChessColor::White);
            pieces[7][4]=new King(ChessColor::White);
            pieces[7][5]=new Bishop(ChessColor::White);
            pieces[7][6]=new Knight(ChessColor::White);
            pieces[7][7]=new Rook(ChessColor::White);
            for(int i=0;i<8;i++)
                pieces[6][i]=new Pawn(ChessColor::White);
        }
    public:
        Board(){
            for(int i=0;i<8;i++)
                for(int j=0;j<8;j++)
                    pieces[i][j]=NULL;
        }
        Peice* getPiece(int row,int column){
            return pieces[row][column];
        }
        void setPiece(int row,int column,Peice* value){
            pieces[row][column]=value;
        }
        Peice* getPiece(Position p){
            return pieces[p.getRow()][p.getColumn()];
        }
        void setPiece(Position p,Peice* value){
            pieces[p.getRow()][p.getColumn()]=value;
        }
        static Board initialize(){
            Board b;
            b.addStartPieces();
            return b;
        }
        static bool isInside(Position p){
            int row=p.getRow();
            int column=p.getColumn();
            if(row>-1 && row<8 && column>-1 && column<8)
                return true;
            return false;
        }

        bool isEmpty(Position p){
            if(getPiece(p)==NULL)
                return true;
            return false;
        }
        vector<Position> piecePositions(){
            vector<Position> positions;
            for(int i=0;i<8;i++){
                for(int j=0;j<8;j++){
                    Position p(i,j);
                    if(!isEmpty(p))
                        positions.push_back(p);
                }
            }
            return positions;
        }
        vector<Position> piecePositionsFor(ChessColor player){
            vector<Position> positions;
            vector<Position> temp=piecePositions();
            int s=temp.size();
            for(int i=0;i<s;i++){
                if(getPiece(temp[i])->getColor()==player){
                    positions.push_back(temp[i]);
                }
            }
            return positions;
        }
        bool isInCheck(ChessColor player){
            ChessColor opponent;
            if(player==ChessColor::White)
                opponent=ChessColor::Black;
            else if(player==ChessColor::Black)
                opponent=ChessColor::White;
            vector<Position> positions=piecePositionsFor(opponent);
            int s=positions.size();
            for(int i=0;i<s;i++){
                Peice* p=getPiece(positions[i]);
                if(p->canCaptureOpponentKing(positions[i],*this)){
                    return true;
                }
            }
            return false;
        }
        Board copy(){
            Board c;
            vector<Position> positions=piecePositions();
            int s=positions.size();
            for(int i=0;i<s;i++){
                c.setPiece(positions[i],getPiece(positions[i])->copy());
            }
            return c;
        }
};

class Move{
    protected:
        MoveType type;
        Position fromPos;
        Position toPos;
    public:
        Move(Position from,Position to):fromPos(from.getRow(),from.getColumn()),toPos(to.getRow(),to.getColumn()){
        }
        MoveType getType(){
            return type;
        }
        Position getFromPosition(){
            return fromPos;
        }
        Position getToPosition(){
            return toPos;
        }
        virtual void execute(Board &board);
        virtual bool isLegal(Board &board){
            ChessColor player=board.getPiece(fromPos)->getColor();
            Board c=board.copy();
            execute(c);
            return !c.isInCheck(player);
        }
};

class MoveNormal:public Move{
    public:
        MoveNormal(Position from,Position to):Move(from,to){
            type=MoveType::Normal;
        }
        void execute(Board &board){
            Peice *p=board.getPiece(fromPos);
            board.setPiece(toPos,p);
            board.setPiece(fromPos,NULL);
            p->setHasMoved(true);
        }

};

class PawnPromotion:public Move{
    private:
        ChessPiece peiceType;
        Peice* createPromotionPeice(ChessColor color){
            switch(peiceType){
                case ChessPiece::KNIGHT:
                    return new Knight(color);
                    break;
                case ChessPiece::BISHOP:
                    return new Bishop(color);
                    break;
                case ChessPiece::ROOK:
                    return new Rook(color);
                    break;
                case ChessPiece::QUEEN:
                    return new Queen(color);
                    break;
            }
        }
    public:
        PawnPromotion(Position from,Position to,ChessPiece t):Move(from,to){
            type=MoveType::Promotion;
            peiceType=t;
        }
        void execute(Board &board){
            Peice *pawn=board.getPiece(fromPos);
            board.setPiece(fromPos,NULL);
            cout<<"2";
            Peice *promotionPeice=createPromotionPeice(pawn->getColor());
            promotionPeice->setHasMoved(true);
            board.setPiece(toPos,promotionPeice);
            cout<<"3";
        }

};

class Castle:public Move{
    private:
        Direction kingMoveDir;
        Position rookFromPos;
        Position rookToPos;
    public:
        Castle(MoveType t,Position kingPos):Move(Position(0,0),Position(0,0)),rookFromPos(0,0),rookToPos(0,0),kingMoveDir(0,0){
            type=t;
            fromPos=kingPos;
            if(type == MoveType::CastelKS){
                kingMoveDir=Direction::getEast();
                toPos.setRow(kingPos.getRow());
                toPos.setColumn(6);
                rookFromPos.setRow(kingPos.getRow());
                rookFromPos.setColumn(7);
                rookToPos.setRow(kingPos.getRow());
                rookToPos.setColumn(5);
            }
            else if(type == MoveType::CastelQS){
                kingMoveDir=Direction::getWest();
                toPos.setRow(kingPos.getRow());
                toPos.setColumn(2);
                rookFromPos.setRow(kingPos.getRow());
                rookFromPos.setColumn(0);
                rookToPos.setRow(kingPos.getRow());
                rookToPos.setColumn(3);
            }
        }
        void execute(Board &board){
           Move *m1=new MoveNormal(fromPos,toPos),*m2=new MoveNormal(rookFromPos,rookToPos);
           m1->execute(board);
           m2->execute(board);
        }
        bool isLegal(Board &board){
            ChessColor player=board.getPiece(fromPos)->getColor();
            if(board.isInCheck(player)){
                return false;
            }
            Board c=board.copy();
            Position kingPos=fromPos;
            for(int i=0;i<2;i++){
                Move *m=new MoveNormal(kingPos,kingPos+kingMoveDir);
                m->execute(c);
                kingPos= kingPos+kingMoveDir;
                if(c.isInCheck(player)){
                    return false;
                }
            }
            return true;
        }
};

class Result{
    private:
        ChessColor winner;
        EndReason reason;
    public:
        Result(ChessColor w,EndReason r){
            winner=w;
            reason=r;
        }
        ChessColor getWinner(){
            return winner;
        }
        EndReason getReason(){
            return reason;
        }
        static Result win(ChessColor player){
            return Result(player,EndReason::CheckMate);
        }
        static Result drarw(EndReason r){
            return Result(ChessColor::None,r);
        }
};

class GameState{
    private:
        Board board;
        ChessColor currentPlayer;
        Result *result;
        void checkIsGameOver(){
            vector<Move*> moves=allLegalMovesFor(currentPlayer);
            if(moves.empty()){
                if(board.isInCheck(currentPlayer)){
                    ChessColor opponent;
                    if(currentPlayer==ChessColor::Black)
                        opponent=ChessColor::White;
                    else
                        opponent=ChessColor::Black;
                    result=new Result(opponent,EndReason::CheckMate);
                }
                else{
                    result=new Result(ChessColor::None,EndReason::StaleMate);
                }
            }
        }
    public:
        GameState(Board b,ChessColor c){
            board=b;
            currentPlayer=c;
            result=NULL;
        }
        Board getBoard(){
            return board;
        }
        ChessColor getCurrentPlayer(){
            return currentPlayer;
        }
        Result* getResult(){
            return result;
        }
        vector<Move*> legalMovesForPieces(Position pos){
            vector<Move*> moves={};
            if(board.isEmpty(pos) || board.getPiece(pos)->getColor() != currentPlayer){
                return moves;
            }
            Peice* piece=board.getPiece(pos);
            moves=piece->getMoves(pos,board);
            int s=moves.size();
            vector<Move*> legalMoves;
            for(int i=0;i<s;i++){
                if(moves[i]->isLegal(board)){
                    legalMoves.push_back(moves[i]);
                }
            }
            return legalMoves;

        }
        void makeMove(Move* mov){
            mov->execute(board);
            if(currentPlayer==ChessColor::Black)
                currentPlayer=ChessColor::White;
            else
                currentPlayer=ChessColor::Black;
            checkIsGameOver();
        }
        vector<Move*> allLegalMovesFor(ChessColor player){
            vector<Position> allPositions=board.piecePositionsFor(player);
            int s=allPositions.size();
            vector<Move*> allMoves,legalMoves;
            for(int i=0;i<s;i++){
                Peice *p=board.getPiece(allPositions[i]);
                vector<Move*> temp=p->getMoves(allPositions[i],board);
                allMoves.insert(allMoves.end(),temp.begin(),temp.end());
            }
            s=allMoves.size();
            for(int i=0;i<s;i++){
                if(allMoves[i]->isLegal(board))
                    legalMoves.push_back(allMoves[i]);
            }
            return legalMoves;
        }
        bool isGameOver(){
            return result!=NULL;
        }
};

class PositionMove{
    private:
        Move *mov;
        Position pos;
    public:
        PositionMove():pos(0,0){

        }
        Move* getMove(){
            return mov;
        }
        Position getPosition(){
            return pos;
        }
        void setMove(Move* m){
            mov=m;
        }
        void setPosition(Position p){
            pos=p;
        }
};

bool Peice::canCaptureOpponentKing(Position from,Board &board){
    vector<Move*> moves=getMoves(from,board);
    int s=moves.size();
    for(int i=0;i<s;i++){
        Peice *p=board.getPiece(moves[i]->getToPosition());
        if(p!=NULL && p->getType()==ChessPiece::KING){
            return true;
        }
    }
    return false;
}

vector<Position> Peice::movePosition(Position from,Board &board,Direction dir){
    vector<Position> moves;
    for(Position pos=from +dir;Board::isInside(pos);pos=pos+dir){
        if(board.isEmpty(pos)){
            moves.push_back(pos);
            continue;
        }
        Peice *p=board.getPiece(pos);
        if(p->getColor()!=color){
            moves.push_back(pos);
        }
        break;
    }
    return moves;
}

vector<Position> Peice::movePositions(Position from,Board &board,Direction dir[],int s){
    vector<Position> totalMoves,directionMoves;
    for(int i=0;i<s;i++){
        directionMoves=movePosition(from,board,dir[i]);
        totalMoves.insert(totalMoves.end(),directionMoves.begin(),directionMoves.end());
    }
    return totalMoves;
}

bool Pawn::canMoveto(Position pos,Board &board){
    return Board::isInside(pos) && board.isEmpty(pos);
}

bool Pawn::canCaputreAt(Position pos,Board &board){
    if(!Board::isInside(pos) || board.isEmpty(pos)){
            return false;
    }
    return board.getPiece(pos)->getColor() != color;
}

vector<Move*> Pawn::promotionMoves(Position from,Position to){
    vector<Move*> moves;
    moves.push_back(new PawnPromotion(from,to,ChessPiece::KNIGHT));
    moves.push_back(new PawnPromotion(from,to,ChessPiece::BISHOP));
    moves.push_back(new PawnPromotion(from,to,ChessPiece::ROOK));
    moves.push_back(new PawnPromotion(from,to,ChessPiece::QUEEN));
    return moves;
}

vector<Move*> Pawn::forwardMoves(Position from,Board &board){
    vector<Move*> moves;
    Position oneMovePos=from+forwardMove;
    if(canMoveto(oneMovePos,board)){
        if(oneMovePos.getRow()==0 || oneMovePos.getRow()==7){
            vector<Move*> temp=promotionMoves(from,oneMovePos);
            moves.insert(moves.end(),temp.begin(),temp.end());
        }
        else{
            moves.push_back(new MoveNormal(from,oneMovePos));
        }
        Position twoMovePos=oneMovePos+forwardMove;
        if(!hasMoved && canMoveto(twoMovePos,board)){
            moves.push_back(new MoveNormal(from,twoMovePos));
        }
    }
    return moves;
}

vector<Move*> Pawn::diagonalMoves(Position from,Board &board){
    vector<Move*> moves;
    Direction dirs[2]={Direction::getEast(),Direction::getWest()};
    for(int i=0;i<2;i++){
        Position to=(from+forwardMove)+dirs[i];
        if(canCaputreAt(to,board)){
            if(to.getRow()==0 || to.getRow()==7){
                vector<Move*> temp=promotionMoves(from,to);
                moves.insert(moves.end(),temp.begin(),temp.end());
            }
            else{
                moves.push_back(new MoveNormal(from,to));
            }
        }
    }
    return moves;
}

vector<Move*> Pawn::getMoves(Position from,Board &board){
    vector<Move*> totalMoves=forwardMoves(from,board),temp=diagonalMoves(from,board);
    totalMoves.insert(totalMoves.end(),temp.begin(),temp.end());
    return totalMoves;
}

bool Pawn::canCaptureOpponentKing(Position from,Board &board){
    vector<Move*> moves=diagonalMoves(from,board);
    int s=moves.size();
    for(int i=0;i<s;i++){
        Peice *p=board.getPiece(moves[i]->getToPosition());
        if(p!=NULL && p->getType()==ChessPiece::KING){
            return true;
        }
    }
    return false;
}

vector<Position> Knight::availablePositions(Position from,Board &board){
    vector<Position> total;
    vector<Position> temp=potentialToPosition(from);
    int s=temp.size();
    for(int i=0;i<s;i++){
        if(Board::isInside(temp[i]) && (board.isEmpty(temp[i]) || board.getPiece(temp[i])->getColor()!=color))
            total.push_back(temp[i]);
    }
    return total;
}

vector<Move*> Bishop::getMoves(Position from,Board &board){
    vector<Move*> totalMoves;
    vector<Position> positions=movePositions(from,board,dirs,4);
    int s=positions.size();
    for(int i=0;i<s;i++){
        Move *temp=new MoveNormal(from,positions[i]);
        totalMoves.push_back(temp);
    }
    return totalMoves;
}

vector<Move*> Knight::getMoves(Position from,Board &board){
    vector<Move*> totalmoves;
    vector<Position> total=availablePositions(from,board);
    int s=total.size();
    for(int i=0;i<s;i++){
        totalmoves.push_back(new MoveNormal(from,total[i]));
    }
    return totalmoves;
}

vector<Move*> Rook::getMoves(Position from,Board &board){
    vector<Move*> totalMoves;
    vector<Position> positions=movePositions(from,board,dirs,4);
    int s=positions.size();
    for(int i=0;i<s;i++){
        Move *temp=new MoveNormal(from,positions[i]);
        totalMoves.push_back(temp);
    }
    return totalMoves;
}

vector<Move*> Queen::getMoves(Position from,Board &board){
    vector<Move*> totalMoves;
    vector<Position> positions=movePositions(from,board,dirs,8);
    int s=positions.size();
    for(int i=0;i<s;i++){
        Move *temp=new MoveNormal(from,positions[i]);
        totalMoves.push_back(temp);
    }
    return totalMoves;
}

bool King::allEmpty(vector<Position> positions,Board &board){
    int s=positions.size();
    for(int i=0;i<s;i++){
        if(!board.isEmpty(positions[i]))
            return false;
    }
    return true;
}

bool King::isUnmovedRook(Position pos,Board &board){
    if(board.isEmpty(pos)){
        return false;
    }
    Peice *p=board.getPiece(pos);
    return p->getType()==ChessPiece::ROOK && !p->getHasMoved();
}

bool King::canCastleKingSide(Position from,Board &board){
    if(hasMoved){
        return false;
    }
    Position rookPos(from.getRow(),7);
    vector<Position> betweenPositions={Position(from.getRow(),5),Position(from.getRow(),6)};
    return isUnmovedRook(rookPos,board) && allEmpty(betweenPositions,board);
}

bool King::canCastleQueenSide(Position from,Board &board){
    if(hasMoved){
        return false;
    }
    Position rookPos(from.getRow(),0);
    vector<Position> betweenPositions={Position(from.getRow(),1),Position(from.getRow(),2),Position(from.getRow(),3)};
    return isUnmovedRook(rookPos,board) && allEmpty(betweenPositions,board);
}

vector<Position> King::availablePositions(Position from,Board &board){
    vector<Position> total;
    for(int i=0;i<8;i++){
        Position to=from+dirs[i];
        if(!Board::isInside(to))
        {
            continue;
        }
        if(board.isEmpty(to)|| board.getPiece(to)->getColor()!=color){
            total.push_back(to);
        }
    }
    return total;
}

vector<Move*> King::getMoves(Position from,Board &board){
    vector<Move*> totalMoves;
    vector<Position> positions=availablePositions(from,board);
    int s=positions.size();
    for(int i=0;i<s;i++){
        totalMoves.push_back(new MoveNormal(from,positions[i]));
    }
    if(canCastleKingSide(from,board)){
        totalMoves.push_back(new Castle(MoveType::CastelKS,from));
    }
    if(canCastleQueenSide(from,board)){
        totalMoves.push_back(new Castle(MoveType::CastelQS,from));
    }

    return totalMoves;
}

bool King::canCaptureOpponentKing(Position from,Board &board){
    vector<Position> positions=availablePositions(from,board);
    int s=positions.size();
    for(int i=0;i<s;i++){
        Peice *p=board.getPiece(positions[i]);
        if(p!=NULL && p->getType()==ChessPiece::KING){
            return true;
        }
    }
    return false;
}

string getImageSource(ChessColor color,ChessPiece peice){
    if(color==ChessColor::Black){
        switch(peice){
            case ChessPiece::PAWN:
                return "Chess Assets/black_pawn.png";
                break;
            case ChessPiece::BISHOP:
                return "Chess Assets/black_bishop.png";
                break;
            case ChessPiece::KNIGHT:
                return "Chess Assets/black_knight.png";
                break;
            case ChessPiece::ROOK:
                return "Chess Assets/black_rook.png";
                break;
            case ChessPiece::QUEEN:
                return "Chess Assets/black_queen.png";
                break;
            case ChessPiece::KING:
                return "Chess Assets/black_king.png";
                break;
        }
    }
    else if(color==ChessColor::White){
        switch(peice){
            case ChessPiece::PAWN:
                return "Chess Assets/white_pawn.png";
                break;
            case ChessPiece::BISHOP:
                return "Chess Assets/white_bishop.png";
                break;
            case ChessPiece::KNIGHT:
                return "Chess Assets/white_knight.png";
                break;
            case ChessPiece::ROOK:
                return "Chess Assets/white_rook.png";
                break;
            case ChessPiece::QUEEN:
                return "Chess Assets/white_queen.png";
                break;
            case ChessPiece::KING:
                return "Chess Assets/white_king.png";
                break;
        }
    }
}

void drawBoard(Texture chessPiecesImages[8][8],Sprite chessPiecesSprites[8][8],Texture &noHighlightTexture,Sprite highlightsSprite[8][8],GameState &gameState){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){
            highlightsSprite[i][j].setTexture(noHighlightTexture);
            highlightsSprite[i][j].setPosition((j*88),(i*88));
            if(!gameState.getBoard().isEmpty(Position(i,j))){
                chessPiecesImages[i][j].loadFromFile(getImageSource(gameState.getBoard().getPiece(i,j)->getColor(),gameState.getBoard().getPiece(i,j)->getType()));
                if(chessPiecesSprites[i][j].getTexture()==NULL){
                chessPiecesSprites[i][j].setTexture(chessPiecesImages[i][j]);
                chessPiecesSprites[i][j].scale(3,3);
                chessPiecesSprites[i][j].setPosition((j*88),(i*88));
                }
            }
        }

}

void cacheMoves(vector<PositionMove> &moveCache,vector<Move*> &moves){
    moveCache.clear();
    int s=moves.size();

    for(int i=0;i<s;i++){
        Position p=moves[i]->getToPosition();
        PositionMove temp;
        temp.setPosition(p);
        temp.setMove(moves[i]);
        moveCache.push_back(temp);
    }
}

void showHighlights(Texture &noHighlightTexture,Sprite highlightsSprite[8][8],vector<PositionMove> &moveCache){
    int s=moveCache.size();
    for(int z=0;z<s;z++){
        Position p=moveCache[z].getPosition();
        int i=p.getRow(),j=p.getColumn();
        highlightsSprite[i][j].setTexture(noHighlightTexture);
        highlightsSprite[i][j].setPosition((j*88),(i*88));
    }
}

void hideHighlights(Texture &highlightsTexture,Sprite highlightsSprite[8][8],vector<PositionMove> &moveCache){
    int s=moveCache.size();
    for(int z=0;z<s;z++){
        Position p=moveCache[z].getPosition();
        int i=p.getRow(),j=p.getColumn();
        highlightsSprite[i][j].setTexture(highlightsTexture);
        highlightsSprite[i][j].setPosition((j*88),(i*88));
    }
}

int main()
{
    RenderWindow window(VideoMode(704,704), "Chess Game",Style::Close);
    GameState gameState(Board::initialize(),ChessColor::White);
    Texture boardImage,chessPiecesImages[8][8],highlightsTexture,noHighlightTexture;
    highlightsTexture.loadFromFile("Chess Assets/avaliable.png");
    noHighlightTexture.loadFromFile("Chess Assets/transparent.png");
    Sprite boardSprite,chessPiecesSprites[8][8],highlightsSprite[8][8];
    vector<PositionMove> moveCache;
    Position *selected=NULL;
    drawBoard(chessPiecesImages,chessPiecesSprites,noHighlightTexture,highlightsSprite,gameState);
    boardImage.loadFromFile("Chess Assets/board.png");
    boardSprite.setTexture(boardImage);
    boardSprite.scale(4,4);
    bool gameIsOver=false,isPromotion=false;
    Position promotionFrom(0,0),promotionTo(0,0);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if(!gameIsOver){
             if(isPromotion){
                if(Keyboard::isKeyPressed(Keyboard::Key::K)){
                    Move *promMove=new PawnPromotion(promotionFrom,promotionTo,ChessPiece::KNIGHT);
                    gameState.makeMove(promMove);
                    drawBoard(chessPiecesImages,chessPiecesSprites,noHighlightTexture,highlightsSprite,gameState);
                    isPromotion=false;
                }
                else if(Keyboard::isKeyPressed(Keyboard::Key::B)){
                    Move *promMove=new PawnPromotion(promotionFrom,promotionTo,ChessPiece::BISHOP);
                    gameState.makeMove(promMove);
                    drawBoard(chessPiecesImages,chessPiecesSprites,noHighlightTexture,highlightsSprite,gameState);
                    isPromotion=false;
                }
                else if(Keyboard::isKeyPressed(Keyboard::Key::R)){
                    Move *promMove=new PawnPromotion(promotionFrom,promotionTo,ChessPiece::ROOK);
                    gameState.makeMove(promMove);
                    drawBoard(chessPiecesImages,chessPiecesSprites,noHighlightTexture,highlightsSprite,gameState);
                    isPromotion=false;
                }
                else if(Keyboard::isKeyPressed(Keyboard::Key::Q)){
                    Move *promMove=new PawnPromotion(promotionFrom,promotionTo,ChessPiece::QUEEN);
                    gameState.makeMove(promMove);
                    drawBoard(chessPiecesImages,chessPiecesSprites,noHighlightTexture,highlightsSprite,gameState);
                    isPromotion=false;
                }
            }
            else{
                if(Mouse::isButtonPressed(Mouse::Left)){
                    Vector2i mousePos= Mouse::getPosition(window);
                    int row=((int)mousePos.y)/88;
                    int column=((int)mousePos.x)/88;
                    if(row>=0 && column>=0){
                        Position p(row,column);
                        if(selected==NULL){
                            vector<Move*> moves=gameState.legalMovesForPieces(p);
                            if(!moves.empty()){
                                selected=new Position(p.getRow(),p.getColumn());
                                cacheMoves(moveCache,moves);
                                showHighlights(highlightsTexture,highlightsSprite,moveCache);
                            }
                        }
                        else{
                            delete selected;
                            selected=NULL;
                            hideHighlights(noHighlightTexture,highlightsSprite,moveCache);
                            int s=moveCache.size();
                            for(int i=0;i<s;i++){
                                if(moveCache[i].getPosition()==p){
                                    if(moveCache[i].getMove()->getType()==MoveType::Promotion){
                                        isPromotion=true;
                                        promotionFrom=moveCache[i].getMove()->getFromPosition();
                                        promotionTo=moveCache[i].getMove()->getToPosition();
                                    }
                                    else{
                                        gameState.makeMove(moveCache[i].getMove());
                                        drawBoard(chessPiecesImages,chessPiecesSprites,noHighlightTexture,highlightsSprite,gameState);
                                    }
                                }
                            }
                        }
                    }

                }
            }
            gameIsOver=gameState.isGameOver();
        }
        else{
                if(Keyboard::isKeyPressed(Keyboard::Key::R)){
                    gameIsOver=false;
                    hideHighlights(noHighlightTexture,highlightsSprite,moveCache);
                    moveCache.clear();
                    GameState game(Board::initialize(),ChessColor::White);
                    gameState=game;
                    drawBoard(chessPiecesImages,chessPiecesSprites,noHighlightTexture,highlightsSprite,gameState);
                    if(selected!=NULL)
                        delete selected;
                    selected=NULL;


                }
        }

        window.clear();

        window.draw(boardSprite);
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++){
                Board b=gameState.getBoard();
                window.draw(highlightsSprite[i][j]);
                if(!b.isEmpty(Position(i,j))){
                    window.draw(chessPiecesSprites[i][j]);
                }

            }
        if(gameIsOver){
            Font font;
            font.loadFromFile("Oxygen-Bold.ttf");
            string endText;
            if(gameState.getResult()->getReason()==EndReason::CheckMate){
                if(gameState.getResult()->getWinner()==ChessColor::White)
                    endText="White wins!";
                else
                    endText="Black wins!";
            }
            else
                endText="it's a draw";

            Text gameoverText(endText,font),restartText("please press R to restart",font);
            gameoverText.setCharacterSize(50);
            restartText.setCharacterSize(35);
            gameoverText.setOrigin(gameoverText.getLocalBounds().width/2.0,0);
            restartText.setOrigin(restartText.getLocalBounds().width/2.0,0);
            gameoverText.setPosition(352,320);
            restartText.setPosition(352,370);
            RectangleShape bg;
            bg.setSize(Vector2f(704,704));
            bg.setFillColor(Color(0,0,0,133));
            window.draw(bg);
            window.draw(gameoverText);
            window.draw(restartText);
        }
        window.display();
    }

    return 0;
}
