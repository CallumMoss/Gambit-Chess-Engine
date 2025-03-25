#include "UCI.hpp"

std::vector<std::string> UCI::split_args(std::string input)
{
    std::vector<std::string> args;

    if(input.size() == 0 ) return {""};

    std::size_t start{0}, end{0};

    while(end < input.size())
    {
        if(input[end++] == ' ')
        {
            args.push_back(input.substr(start, end - start - 1));
            start = end;
        }
    }

    if(input[input.size() - 1] != ' ')
        args.push_back(input.substr(start, end - start));

    return args;
}

// Helpful comment from: https://github.com/AndyGrant/Ethereal/blob/master/src/uci.c
    /*
    |------------|-----------------------------------------------------------------------|
    |  Commands  | Response. * denotes that the command blocks until no longer searching |
    |------------|-----------------------------------------------------------------------|
    |        uci |           Outputs the engine name, authors, and all available options |
    |    isready | *           Responds with readyok when no longer searching a position |
    | ucinewgame | *  Resets the TT and any Hueristics to ensure determinism in searches |
    |   position | *  Sets the board position via an optional FEN and optional move list |
    |         go | *       Searches the current position with the provided time controls |
    |       quit |             Exits the engine and any searches by killing the UCI loop |
    |------------|-----------------------------------------------------------------------|
    */

// Inspired by https://github.com/AndyGrant/Ethereal/blob/master/src/uci.c#L133
void UCI::go(std::vector<std::string>& args, Timer& timer, Position& pos, Transposition_Table& tt, PositionStack& ps, bool is_gambit, Opponent& opp, std::vector<EvaluatedMove>& evaluated_opp_responses) {
    // Give default values which are updated later
    // Usually these values for wtime and btime should always be updated, so value here is technically irrelevant
    u64 wtime = 60'000; // white has x msec left on the clock
    u64 btime = 60'000; // black has x msec left on the clock
    u64 winc = 0; // white increment per move in mseconds if x > 0
    u64 binc = 0; // black increment per move in mseconds if x > 0

    for(size_t i = 0; i < args.size(); i++) {
        if (args[i] == "wtime") { // update fields by turning the string input to a u64
            wtime = std::stoull(args[i + 1]);
        }
        else if (args[i] == "btime") {
            btime = std::stoull(args[i + 1]);
        }
        else if (args[i] == "winc") {
            winc = std::stoull(args[i + 1]);
        }
        else if (args[i] == "binc") {
            binc = std::stoull(args[i + 1]);
        }
    }

    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }

    timer.start_timer();

    Search search = Search(is_gambit, opp);
    search.iterative_deepening(pos, timer, tt, ps, evaluated_opp_responses);
    Move best_move = search.get_root_best_move();
    assert((!best_move.equals(Utils::NULL_MOVE))  && "Null move attempted to be played.");
    std::string best_move_str = Utils::move_to_board_notation(best_move);
    std::cout << "bestmove " << best_move_str << std::endl;
}

void UCI::position(std::vector<std::string>& args, Position& pos, PositionStack& ps, bool is_gambit, Opponent& opp, std::vector<EvaluatedMove>& evaluated_opp_responses) {
    if(args[1] == "fen") {
        std::string constructed_fen{""};

        for(std::size_t i{2}; i < 7; ++i) {
            constructed_fen += std::string(args[i]) + ' ';
        }

        constructed_fen += std::string(args[7]);

        pos = Position(constructed_fen);
    }
    else if (args[1] == "startpos") {
        pos = Position();
    }
    else {
        std::cerr << "Invalid UCI position input";
        std::exit(-1);
    }

    int index = -1;

    for(int i = 0; i < static_cast<int>(args.size()); i++) {
        if(args[i] == "moves") {
            index = i;
            break;
        }
    }
    if(index == 0) opp.set_colour(Turn::BLACK); // if we are asked to play and no moves have been played then we are white
    else if(index == 1) opp.set_colour(Turn::WHITE);

    if(index != -1) { // if there is a moves argument
        for(index++; index < static_cast<int>(args.size()); index++) {
            Move move = pos.board_notation_to_move(args[index]);

            pos.make_move(move);
            // add game history
            ps.push_back(pos.get_zobrist_key());
        }

        if(is_gambit)
        { // Adjust opponent skill based on their move
            if(!evaluated_opp_responses.empty()) // for if opp plays first
            {
                Move move_played = pos.board_notation_to_move(args.back());
                for(int i = 0; i < evaluated_opp_responses.size(); i++)
                {
                    if(move_played.equals(evaluated_opp_responses[i].move))
                    {
                        opp.update_skill(i, evaluated_opp_responses.size());
                    }
                }
            }
        }
    }
    //else { return; }
}

int UCI::options(std::vector<std::string>& args) {
    if(args[2] == "Hash") {
        return std::stoi(args[3]); // returns the number of megabytes used for the transposition table
    }
}