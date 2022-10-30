/*
Copyright 2022 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

---------------------------------------------------------------------------------------------------------------------
SadBoat Engine:
    The SadBoat Engine is an open-source, free to use game engine being designed in C++.
    Most of it's functionality is defined in it's header files, making it a mostly header only engine.
    Consequently, you do not need this main.cpp, specifically, to run your game. It is, however, recommended.
    Opting to not use this main.cpp file means you may need to reimplement the code found herein. This is up to your discretion.

Disclaimer:
    We are not responsible for what you do with our code. You can use it as intended, or make your own use of it.
    If you decide to use it within the parameters we've defined, our documentation exists to help you!
    If something is not clear in our documentation, you can email contact@sadboat.com and we'll try and clarify.
    If your usage is outside of the scope of our documentation, we may not be able to provide you any helpful results.
---------------------------------------------------------------------------------------------------------------------
*/

//
//  Engine Defines
//
//  This adds RapidXML and Vulkan to our include path
//  You can disable these if you'd like, but I'd suggest against it.
//
#define INCLUDE_RAPIDXML
#define INCLUDE_VULKAN

//
//  Engine Include
//
//  This file contains all the includes for the engine. 
//  It's all you need to get started.
//
//----------------------------------
#include "sb-engine.hpp"

using namespace std;
using namespace SBE;

//A short test driver to determine window name and whether to boot Catch.
string testSetup(int argc, char* argv[], string possibleName)
{
    //Declare an empty name.
    string name = "";
    int results;

    //Write a long line for visual clarity.
    int i = 100;
    while(i>0)
    {
        std::cout<<"-";
        i--;
    }
    std::cout<<endl;

    
    //This code only gets included into our program if we run "make" or "make prod" during compilation.
    #ifdef CONFIG_PROD
        std::cout<<"Launching engine...\n";
        name = possibleName;
    #endif

    //This code only gets included into our program if we run "make test" during compilation.
    #ifdef CONFIG_TEST
        //If we ran the catch session, the results variable may change, indicating a failed test.
        //Do not launch the game if the tests fail.

        std::cout<<"Running tests...\n";
        results = Catch::Session().run(argc, argv);
        if(results!=0)
        {
            std::cout<<"Tests failed, engine aborting.\n";
            throw new exception();
        }

        std::cout<<"Tests succeeded, engine launching.\n";
        name = "Test Stage - ";
        name+=possibleName;
        
    #endif

    return name;
}

//----------------------------------
//  main(int argc, char* argv[]) function:
//
//      Entry point for program. Manages testing runtimes, and launches other session managers.
//      Return point for all session managers, processes engine shutdown.
//
//----------------------------------
int main(int argc, char* argv[])
{
    //  Unit Test Setup
    //----------------------------------

    //gameName: A constant identifier for what our game should be called.
    const string gameName = "SadBoat Engine";

    //name: An identifier for what our game will actually be called. Dependent on test cases and compilation status.
    string name = testSetup(argc, argv, gameName);

    enum pType
    {
        PAWN=1,
        KNIGHT=3,
        BISHOP=3,
        ROOK=5,
        QUEEN=9,
        KING=0
    };

    class Piece;
    class Board;

    //
    //  Move Lookup
    //
    //----------------------------------------
    //


    unordered_map<pType, Event<vector<Point>,vector<Point>, Piece, Board>*> movFuncs;

    struct Board
    {
        unordered_map<int,unordered_map<int,Piece*>> board;
    };

    class Piece
    {
        private:
        unordered_map<pType, Event<vector<Point>,vector<Point>, Piece, Board>*>* movFuncs;


        public:
        pType piece;
        Point location;
        bool isWhite;


        Piece()
        {

        }

        Piece(unordered_map<pType, Event<vector<Point>,vector<Point>, Piece, Board>*>* movFuncsPtr, pType pieceType, Point location, bool isWhite)
        {
            movFuncs=movFuncsPtr;
            piece=pieceType;
            this->location=location;
            this->isWhite=isWhite;
        }


        vector<Point> getMoves(Board boardState)
        {
            return (*this->movFuncs)[piece]->call(vector<Point>(), (*this), boardState);
        }
    };


    

    

    //
    //  Move Cleaners
    //
    //----------------------------------------
    //

    //Remove moves that are out of bounds
    Event<vector<Point>,vector<Point>> normalizeMoves(
        F(vector<Point> board)
        {
            vector<Point> newBoard;
            Range boardRange(1,8);

            for(int i = 0; i<board.size(); i++)
            {
                if(boardRange[board[i].getX()] && boardRange[board[i].getY()])
                {
                    newBoard.push_back(board[i]);
                }
            }

            return newBoard;
        }
    );

    #define nF(a...) [&normalizeMoves](a...)

    //Remove duplicate moves
    Event<vector<Point>,vector<Point>> removeDuplicates(
        nF(vector<Point> oldBoard)
        {
            vector<Point> newBoard;
            
            oldBoard = normalizeMoves(oldBoard);

            unordered_map<int,unordered_map<int,bool>> xymap;
            for(int x=1; x<9; x++)
            {
                for(int y=1; y<9; y++)
                {
                    xymap[x][y]=false;
                }
            }

            for(int i = 0; i<oldBoard.size(); i++)
            {
                int nX = oldBoard[i].getX();
                int nY = oldBoard[i].getY();

                if(!xymap[nX][nY])
                {
                    xymap[nX][nY]=true;
                    newBoard.push_back(oldBoard[i]);
                }
            }

            return newBoard;
        }
    );

    Event<vector<Point>, vector<Point>, Piece, Board> removeCollisions
    (
        [](vector<Point> oldMoves, Piece piece, Board oldBoard)
        {

            //TEMP
            //ADD CHESS COLLISION HERE

            return oldMoves;
        }
    );

    Event<vector<Point>,vector<Point>, Piece, Board> cleanMoves(
        [&removeCollisions, &removeDuplicates, &normalizeMoves](vector<Point> oldMoves, Piece piece, Board oldBoard){
            return removeCollisions(removeDuplicates(normalizeMoves(oldMoves)), piece, oldBoard);
        }
    );



    //
    //  Move Calculators
    //
    //----------------------------------------
    //


    //Doesn't check for en passat!
    movFuncs[PAWN]=new Event<vector<Point>, vector<Point>, Piece, Board>(
        nF(vector<Point> oldMoves, Piece chessPiece, Board boardState)
        {
            vector<Point> moves;

            auto location=chessPiece.location;
            auto isWhite=chessPiece.isWhite;

            (isWhite) ? 
                moves.push_back(Point(location.getX(),location.getY()+1,0)) :
                moves.push_back(Point(location.getX(),location.getY()-1,0)) ;
            
            (isWhite) ? 
                (location.getX()==2) ? 
                    moves.push_back(Point(location.getX(),location.getY()+2,0)) : []{}() : 
                (location.getX()==7) ? 
                    moves.push_back(Point(location.getX(),location.getY()-2,0)) : []{}() ;

            Piece* uR = boardState.board[location.getX()+1][location.getY()+1];
            Piece* dR = boardState.board[location.getX()+1][location.getY()-1];
            Piece* uL = boardState.board[location.getX()-1][location.getY()+1];
            Piece* dL = boardState.board[location.getX()-1][location.getY()-1];

            if(isWhite && (uR || uL) && (!uR->isWhite || !uL->isWhite))
            {
                if(uR && !uR->isWhite) moves.push_back(uR->location);
                if(uL && !uL->isWhite) moves.push_back(uL->location);
            }
            if(!isWhite && (dR || dL) && (dR->isWhite || dL->isWhite))
            {
                if(dR && dR->isWhite) moves.push_back(dR->location);
                if(dL && dL->isWhite) moves.push_back(dL->location);
            }

            return normalizeMoves(moves);
        }
    );

    movFuncs[KNIGHT]=new Event<vector<Point>, vector<Point>, Piece, Board>(
        nF(vector<Point> oldBoard, Piece chessPiece, Board boardState)
        {
            vector<Point> moves;

            auto location = chessPiece.location;

            moves.push_back(location+Point(1,2,0));
            moves.push_back(location+Point(1,-2,0));
            moves.push_back(location+Point(2,1,0));
            moves.push_back(location+Point(2,-1,0));

            moves.push_back(location-Point(1,2,0));
            moves.push_back(location-Point(1,-2,0));
            moves.push_back(location-Point(2,1,0));
            moves.push_back(location-Point(2,-1,0));

            return normalizeMoves(moves);
        }
    );

    movFuncs[BISHOP]=new Event<vector<Point>, vector<Point>, Piece, Board>(
        nF(vector<Point> oldBoard, Piece chessPiece, Board boardState)
        {
            vector<Point> moves;
            for(int i = 0; i<8; i++)
            {
                moves.push_back(chessPiece.location + Point(i,i,0));
                moves.push_back(chessPiece.location - Point(i,i,0));

                moves.push_back(chessPiece.location + Point(-i,i,0));
                moves.push_back(chessPiece.location - Point(-i,i,0));
            }

            return normalizeMoves(moves);
        }
    );

    //Doesn't check for castling!
    movFuncs[ROOK]=new Event<vector<Point>, vector<Point>, Piece, Board>(
        nF(vector<Point> oldBoard, Piece chessPiece, Board boardState)
        {
            vector<Point> moves;
            for(int i = 0; i<8; i++)
            {
                moves.push_back(chessPiece.location+Point(i,0,0));
                moves.push_back(chessPiece.location-Point(i,0,0));
                moves.push_back(chessPiece.location+Point(0,i,0));
                moves.push_back(chessPiece.location-Point(0,i,0));
            }

            return normalizeMoves(moves);
        }
    );

    //Doesn't check for check/checkmate!
    movFuncs[KING]=new Event<vector<Point>, vector<Point>, Piece, Board>(
        nF(vector<Point> oldBoard, Piece chessPiece, Board boardState)
        {
            vector<Point> moves;

            for(int x=-1; x<3; x++)
            {
                for(int y=-1; y<3; y++)
                {
                    if(Point(x,y,0)!=chessPiece.location)
                    {
                        moves.push_back(Point(x,y,0));
                    }
                }
            }

            return normalizeMoves(moves);
        }
    );

    movFuncs[QUEEN]=new Event<vector<Point>, vector<Point>, Piece, Board>(
        [&movFuncs, &cleanMoves](vector<Point> oldBoard, Piece chessPiece, Board boardState)
        {
            vector<Point> moves;

            vector<Point> king = movFuncs[KING]->call(oldBoard, chessPiece, boardState);
            vector<Point> bishop = movFuncs[BISHOP]->call(oldBoard, chessPiece, boardState);
            vector<Point> rook = movFuncs[ROOK]->call(oldBoard, chessPiece, boardState);

            for(int i=0; i<king.size(); i++)
            {
                moves.push_back(king[i]);
            }
            for(int i=0; i<bishop.size(); i++)
            {
                moves.push_back(bishop[i]);
            }
            for(int i=0; i<rook.size(); i++)
            {
                moves.push_back(rook[i]);
            }

            moves = cleanMoves(moves, chessPiece, boardState);
            return moves;
        }
    );

    //*/

    /*
    rapidxml::xml_document<> document;
    rapidxml::xml_node<>* root;

    ifstream testData("./config/quotes.xml");

    vector<char> buffer((istreambuf_iterator<char>(testData)),istreambuf_iterator<char>());
    buffer.push_back('\0');

    document.parse<0>(&buffer[0]);
    root = document.first_node("QuotesList");
    
    auto Fishing = root->first_node("Fishing");
    auto Real = root->first_node("Real");

    std::cout<<"Fishing Quotes: "<<endl;
    for(int i = 0; i<50; i++) 
    {
        std::cout<<"-";
    }
    std::cout<<endl;
    
    //For some node "quote" as a child of Fishing
    //  Where quote exists and is not a nullptr, do some thing
    //  And move to it's sibling
    for(rapidxml::xml_node<>* quote = Fishing->first_node("Quote"); quote; quote=quote->next_sibling())
    {
        std::cout<<quote->value()<<endl;
    }

    std::cout<<"\n\nReal Quotes: "<<endl;
    for(int i = 0; i<50; i++) 
    {
        std::cout<<"-";
    }
    std::cout<<endl;

    //For some node "quote" as a child of Real
    //  Where quote exists and is not a nullptr, do some thing
    //  And move to it's sibling.
    for(rapidxml::xml_node<>* quote = Real->first_node("Quote"); quote; quote=quote->next_sibling())
    {
        std::cout<<quote->value()<<endl;
    }


    //*/


    Piece rookTest = Piece(&movFuncs, ROOK, Point(3,3,0), true);

    Board testBoard;
    testBoard.board[3][3]=&rookTest;

    /*

    auto moves = rookTest.getMoves(testBoard);*/

    auto moves = movFuncs[ROOK]->call(vector<Point>(), rookTest, testBoard);

    moves = cleanMoves(moves, rookTest, testBoard);

    for(int i = 0; i<moves.size(); i++)
    {
        cout<<" x: "<<moves[i].getX()<<" y: "<<moves[i].getY()<<endl;
    }

    return 0;
}
