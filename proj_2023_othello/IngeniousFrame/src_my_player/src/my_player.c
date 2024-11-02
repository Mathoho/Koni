/* vim: :se ai :se sw=4 :se ts=4 :se sts :se et */

/*H**********************************************************************
 *
 *    This is a skeleton to guide development of Othello engines that can be used
 *    with the Ingenious Framework and a Tournament Engine. 
 *
 *    The communication with the referee is handled by an implementaiton of comms.h,
 *    All communication is performed at rank 0.
 *
 *    Board co-ordinates for moves start at the top left corner of the board i.e.
 *    if your engine wishes to place a piece at the top left corner, 
 *    the "gen_move_master" function must return "00".
 *
 *    The match is played by making alternating calls to each engine's 
 *    "gen_move_master" and "apply_opp_move" functions. 
 *    The progression of a match is as follows:
 *        1. Call gen_move_master for black player
 *        2. Call apply_opp_move for white player, providing the black player's move
 *        3. Call gen move for white player
 *        4. Call apply_opp_move for black player, providing the white player's move
 *        .
 *        .
 *        .
 *        N. A player makes the final move and "game_over" is called for both players
 *    
 *    IMPORTANT NOTE:
 *        Write any (debugging) output you would like to see to a file. 
 *        	- This can be done using file fp, and fprintf()
 *        	- Write a method to make this easier
 *        In a multiprocessor version 
 *        	- each process should write debug info to its own file 
 *H***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include <mpi.h>
#include <time.h>
#include <assert.h>
#include "comms.h"

#include <limits.h> 
const int EMPTY = 0;
const int BLACK = 1;
const int WHITE = 2;

const int OUTER = 3;
const int ALLDIRECTIONS[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
const int BOARDSIZE = 100;

const int LEGALMOVSBUFSIZE = 65;
const char piecenames[4] = {'.','b','w','?'};

void run_master(int argc, char *argv[]);
int initialise_master(int argc, char *argv[], int *time_limit, int *my_colour, FILE **fp);
void gen_move_master(char *move, int my_colour, FILE *fp);
void apply_opp_move(char *move, int my_colour, FILE *fp);
void game_over();
void run_worker();
void initialise_board();
void free_board();

void legal_moves(int player, int *moves, FILE *fp);
int legalp(int move, int player, FILE *fp);
int validp(int move);
int would_flip(int move, int dir, int player, FILE *fp);
int opponent(int player, FILE *fp);
int find_bracket_piece(int square, int dir, int player, FILE *fp);
int random_strategy(int my_colour, FILE *fp);
void make_move(int move, int player, FILE *fp);
void make_flips(int move, int dir, int player, FILE *fp);
int get_loc(char* movestring);
void get_move_string(int loc, char *ms);
void print_board(FILE *fp);
char nameof(int piece);
int count(int player, int * board);

int my_strategy(int my_colour, FILE *fp);
void update_disp(int disp[], int count[], int size);
void init_zero(int disp[],int count[],int size);
int*  make_move_local(int move, int player, int *loc_board);
void make_flips_local(int move, int dir, int player, int *loc_board);
int would_flip_local(int move, int dir, int player, int *loc_board);
int find_bracket_piece_local(int square, int dir, int player,int *loc_board);
int opponent_local(int player);
void legal_moves_local(int player, int *moves, int *loc_board);
int legalp_local(int move, int player, int *loc_board);
void gen_move_worker(int my_rank, int size, int loc_mycolour);
int validp_local(int move);
void copy_board(int *copy, int *origional);
void apply_move(int *loc_board,int mv,int player);
int minmaxV3(int *loc_board, int player, int *chosen_move, int alpha, int beta,int loc_mycolour,int move[],int len,int my_rank);
int minmax_value2(int *loc_board,int original_turn, int current_turn,int depth,int a,int b,int loc_mycolour,int move[],int len,int my_rank);

int* pice_diff(int *f_d, int *loc_board, int loc_myclour);
int cor_pice(int *loc_board, int loc_myclour);
int close_tocor(int *loc_board, int loc_myclour);
int mobility(int *loc_board, int loc_mycolour);


#define MAX_DEPTH 4

int *board;

int main(int argc, char *argv[]) {
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
	initialise_board(); //one for each process
	
	if (rank == 0) {
		run_master(argc, argv);
	} else {
	    run_worker(rank);
		
	}
	MPI_Barrier(MPI_COMM_WORLD);
	game_over();
}

void run_master(int argc, char *argv[]) {
	char cmd[CMDBUFSIZE];
	char my_move[MOVEBUFSIZE];
	char opponent_move[MOVEBUFSIZE];
	int time_limit;
	int my_colour;
	int running = 0;
	FILE *fp = NULL;
	
	if (initialise_master(argc, argv, &time_limit, &my_colour, &fp) != FAILURE) {
		running = 1;
	}
	if (my_colour == EMPTY) my_colour = BLACK;
	// Broadcast my_colour
	MPI_Bcast(&my_colour,1,MPI_INT,0,MPI_COMM_WORLD);

	//TODO broadcast running maybe
	while (running == 1) {
		
		/* Receive next command from referee */
		if (comms_get_cmd(cmd, opponent_move) == FAILURE) {
			fprintf(fp, "Error getting cmd\n");
			
			fflush(fp);
			running = 0;
			break;
		}
		

		/* Received game_over message */
		if (strcmp(cmd, "game_over") == 0) {
			running = 0;
			fprintf(fp, "Game over\n");
			fflush(fp);
			break;

		/* Received gen_move message */
		} else if (strcmp(cmd, "gen_move") == 0) {
			
			//Broadcast running 
			MPI_Bcast(&running,1,MPI_INT,0,MPI_COMM_WORLD);
		
			//Broadcast board 
			MPI_Bcast(board,BOARDSIZE,MPI_INT,0,MPI_COMM_WORLD); // change to one
			
			gen_move_master(my_move, my_colour, fp);
			
			print_board(fp);

	
			if (comms_send_move(my_move) == FAILURE) { 
				running = 0;
				fprintf(fp, "Move send failed\n");
				fflush(fp);
				break;
			}
		/* Received opponent's move (play_move mesage) */
		} else if (strcmp(cmd, "play_move") == 0) {
			apply_opp_move(opponent_move, my_colour, fp);
			print_board(fp);
		/* Received unknown message */
		} else {
			fprintf(fp, "Received unknown command from referee\n");
		}
		
	}
	
	//Broadcast running
	MPI_Bcast(&running,1,MPI_INT,0,MPI_COMM_WORLD);
}

int initialise_master(int argc, char *argv[], int *time_limit, int *my_colour, FILE **fp) {
	int result = FAILURE;

	if (argc == 5) { 
		unsigned long ip = inet_addr(argv[1]);
		int port = atoi(argv[2]);
		*time_limit = atoi(argv[3]);

		*fp = fopen(argv[4], "w");
		if (*fp != NULL) {
			fprintf(*fp, "Initialise communication and get player colour \n");
			if (comms_init_network(my_colour, ip, port) != FAILURE) {
				result = SUCCESS;
			}
			fflush(*fp);
		} else {
			fprintf(stderr, "File %s could not be opened", argv[4]);
		}
	} else {
		fprintf(*fp, "Arguments: <ip> <port> <time_limit> <filename> \n");
	}
	
	return result;
}

void initialise_board() {
	int i;
	board = (int *) malloc(BOARDSIZE * sizeof(int));
	for (i = 0; i <= 9; i++) board[i] = OUTER;
	for (i = 10; i <= 89; i++) {
		if (i%10 >= 1 && i%10 <= 8) board[i] = EMPTY; else board[i] = OUTER;
	}
	for (i = 90; i <= 99; i++) board[i] = OUTER;
	board[44] = WHITE; board[45] = BLACK; board[54] = BLACK; board[55] = WHITE;
}

void free_board() {
	free(board);
}

void legal_moves_local(int player, int *moves, int *loc_board) {
	int move, i;
	moves[0] = 0;
	i = 0;
	for (move = 11; move <= 88; move++) {
		if (legalp_local(move, player,loc_board)) {
      		i++;
    		moves[i] = move; //storing the moves in the array
    	}
	}
	moves[0] = i; //number of moves that can be made
}

int legalp_local(int move, int player, int *loc_board) {
	int i;
	if (!validp_local(move)) return 0;
	if (loc_board[move] == EMPTY) {
		i = 0;
		while (i <= 7 && !would_flip_local(move, ALLDIRECTIONS[i], player, loc_board)) i++;
		if (i == 8) return 0; else return 1;
	}
	else return 0;
}

int validp_local(int move) {

	if ((move >= 11) && (move <= 88) && (move%10 >= 1) && (move%10 <= 8))
		return 1;
	else return 0;
}


int * weight_it(int *w_b){
	int i;
	for (i = 10; i < 89; i++)
	{
		if(i%10 >= 1 && i%10 <= 8){
			w_b[i] = 1;
		}
	}
	
	return w_b;
}

void w_edge_p(int *b,int start1, int end1,int incr,int weight,int start2,int end2){
	for (int i = start1 ; i <= end1; i+=incr)
	{
		b[i] = weight;
	}

	for (int i = start2 ; i <= end2; i+=incr)
	{
		b[i] = weight;
	}

}

/*
*https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
*/

int eval_board(int *loc_board, int loc_mycolour){
	
	int point = 0;
	int my_piece = count(loc_mycolour,loc_board);
	int opp_colour = opponent_local(loc_mycolour); 
	int opp_piece = count(opp_colour,loc_board);
	if (my_piece > opp_piece)
	{
		point = (100.0 * my_piece)/(my_piece + opp_piece);
	}else if (my_piece < opp_piece)
	{
		point = -(100.0 * opp_piece)/(my_piece + opp_piece);
	}

	int *frp_d = (int *) malloc(2*sizeof(int)); 
	frp_d = pice_diff(frp_d,loc_board,loc_mycolour); 
	
	int cor = cor_pice(loc_board,loc_mycolour);
	int cl_cor = close_tocor(loc_board,loc_mycolour);
	int mobi = mobility(loc_board,loc_mycolour);
	int score = (10*point)+(802*cor)+(382*cl_cor)+(79*mobi)+(74*frp_d[0])+(10*frp_d[1]);
	return score;
}


int* weight_brd(int *w_board){

	int i = 11;//row 1
	w_board[i] = 20; i+=1;  w_board[i] = -3; i+=1;  w_board[i] = 11; i+=1;
	w_board[i] = 8; i+=1;   w_board[i] = 8; i+=1;   w_board[i] = 11; i+=1;
	w_board[i] = -3; i+=1;   w_board[i] = 20; 

	i+=1;//row 2
	w_board[i] = -3; i+=1;  w_board[i] = -7; i+=1;  w_board[i] = -4; i+=1;
	w_board[i] = 1; i+=1;   w_board[i] = 1; i+=1;   w_board[i] = -4; i+=1;
	w_board[i] = -7; i+=1;   w_board[i] = -3;

	i+=1;//row 3
	w_board[i] = 11; i+=1;  w_board[i] = -4; i+=1;  w_board[i] = 2; i+=1;
	w_board[i] = 2; i+=1;   w_board[i] = 2; i+=1;   w_board[i] = 2; i+=1;
	w_board[i] = -4; i+=1;   w_board[i] = 11;

	i+=1;//row 4
	w_board[i] = 8; i+=1;  w_board[i] = 1; i+=1;  w_board[i] = 2; i+=1;
	w_board[i] = -3; i+=1;   w_board[i] = -3; i+=1;   w_board[i] = 2; i+=1;
	w_board[i] = 1; i+=1;   w_board[i] = 8;

	i+=1;//row 5
	w_board[i] = 8; i+=1;  w_board[i] = 1; i+=1;  w_board[i] = 2; i+=1;
	w_board[i] = -3; i+=1;   w_board[i] = -3; i+=1;   w_board[i] = 2; i+=1;
	w_board[i] = 1; i+=1;   w_board[i] = 8;

	i+=1;//row 6
	w_board[i] = 11; i+=1;  w_board[i] = -4; i+=1;  w_board[i] = 2; i+=1;
	w_board[i] = 2; i+=1;   w_board[i] = 2; i+=1;   w_board[i] = 2; i+=1;
	w_board[i] = -4; i+=1;   w_board[i] = 11;

	i+=1;//row 7
	w_board[i] = -3; i+=1;  w_board[i] = -7; i+=1;  w_board[i] = -4; i+=1;
	w_board[i] = 1; i+=1;   w_board[i] = 1; i+=1;   w_board[i] = -4; i+=1;
	w_board[i] = -7; i+=1;   w_board[i] = -3;

	i+=1;//row 8
	w_board[i] = 20; i+=1;  w_board[i] = -3; i+=1;  w_board[i] = 11; i+=1;
	w_board[i] = 8; i+=1;   w_board[i] = 8; i+=1;   w_board[i] = 11; i+=1;
	w_board[i] = -3; i+=1;   w_board[i] = 20;

	return w_board;
}
int* pice_diff(int *f_d, int *loc_board, int loc_myclour){
	/*
	checks its eight neighboring elements (represented by the arrays X1 and Y1) 
	to see if any of them are empty. If a neighboring element is empty, 
	then the code increments either the my_front_tiles or opp_front_tiles
	variable, depending on whether the current element belongs to the playeror opponent. 
	The loop then moves on to the next element.
	*/
	int x = 0;
	int opp_colour = opponent_local(loc_myclour);
	int *w_board = (int *) malloc(89*sizeof(int));
	w_board = weight_brd(w_board);
	int my_piece = 0;
	int my_frpiece = 0;
	int opp_piece = 0;
	int opp_frpiece = 0;
	int frp = 0;
	int d_wght = 0;
	int X[] = {-11,-10,-9,-1,0,1,9,10,11};
	
	for (int i = 10; i <= 89; i++) {
		if (i%10 >= 1 && i%10 <= 8){
			if (loc_board[i] == loc_myclour)
			{
				d_wght += w_board[i];
				my_piece+=1;
			}else if (loc_board[i] == opp_colour)
			{
				d_wght -= w_board[i];
				opp_piece+=1;
			}
			
			if (loc_board[i] != EMPTY)
			{
				for (int k = 0; i < 8; k++)
				{
					x = i + X[k];
					if(x%10 >= 10 && x%10<= 8){
						if (x >= 11 && x <= 89  && loc_board[x] == EMPTY)
						{
							if (loc_board[i] == loc_myclour)
							{
								my_frpiece+=1;
							}else if(loc_board[i] == opp_colour){
								opp_frpiece+=1;
							}
						
						}
					}
					
					
				}
				
			}
			
		}	

	}

	if (my_frpiece > opp_frpiece)
	{
		frp = -(100 * my_frpiece)/(my_frpiece + opp_frpiece);;
	}else if(my_frpiece < opp_frpiece){
		frp = (100 * opp_frpiece)/(my_frpiece + opp_frpiece);
	}
	f_d[0] = frp;
	f_d[1] = d_wght;
	return f_d;
	
}

int cor_pice(int *loc_board, int loc_myclour)
{
	int opp_colour = opponent_local(loc_myclour);
	
	int my_pieces = 0;
	int opp_pices = 0;
	if(loc_board[11] == loc_myclour) my_pieces++;
	else if(loc_board[11] == opp_colour) opp_pices++;
	if(loc_board[18] == loc_myclour) my_pieces++;
	else if(loc_board[18] == opp_colour) opp_pices++;
	if(loc_board[81] == loc_myclour) my_pieces++;
	else if(loc_board[81] == opp_colour) opp_pices++;
	if(loc_board[88] == loc_myclour) my_pieces++;
	else if(loc_board[88] == opp_colour) opp_pices++;
	int c = 25 * (my_pieces - opp_pices);
	return c;
}

int close_tocor(int *loc_board, int loc_myclour){
	int my_pieces = 0;
	int opp_pices = 0;
	int opp_colour = opponent_local(loc_myclour);
	
	//if corner piece is empty
	if (loc_board[11]  == EMPTY)
	{
		if(loc_board[12] == loc_myclour) my_pieces++;
		else if(loc_board[12] == opp_colour) opp_pices++;
		if(loc_board[21] == loc_myclour) my_pieces++;
		else if(loc_board[21] == opp_colour) opp_pices++;
		if(loc_board[22] == loc_myclour) my_pieces++;
		else if(loc_board[22] == opp_colour) opp_pices++;
	}

	if (loc_board[18] == EMPTY)
	{
		if(loc_board[17] == loc_myclour) my_pieces++;
		else if(loc_board[17] == opp_colour) opp_pices++;
		if(loc_board[27] == loc_myclour) my_pieces++;
		else if(loc_board[27] == opp_colour) opp_pices++;
		if(loc_board[28] == loc_myclour) my_pieces++;
		else if(loc_board[28] == opp_colour) opp_pices++;
	}

	if (loc_board[81] == EMPTY)
	{
		if(loc_board[72] == loc_myclour) my_pieces++;
		else if(loc_board[72] == opp_colour) opp_pices++;
		if(loc_board[71] == loc_myclour) my_pieces++;
		else if(loc_board[71] == opp_colour) opp_pices++;
		if(loc_board[82] == loc_myclour) my_pieces++;
		else if(loc_board[82] == opp_colour) opp_pices++;
	}

	if (loc_board[88] == EMPTY)
	{
		if(loc_board[87] == loc_myclour) my_pieces++;
		else if(loc_board[87] == opp_colour) opp_pices++;
		if(loc_board[78] == loc_myclour) my_pieces++;
		else if(loc_board[78] == opp_colour) opp_pices++;
		if(loc_board[77] == loc_myclour) my_pieces++;
		else if(loc_board[77] == opp_colour) opp_pices++;
	}


	int lloc = 12.5 * (my_pieces - opp_pices);
	return lloc;
}


int mobility(int *loc_board, int loc_mycolour){
	int my_pieces = 0;
	int opp_pieces = 0;
	int m = 0;
	int opp_colour = opponent_local(loc_mycolour);
	int *my_lglmoves = (int *) malloc(LEGALMOVSBUFSIZE*sizeof(int));
	int *opp_lglmoves = (int *) malloc(LEGALMOVSBUFSIZE*sizeof(int));
	legal_moves_local(loc_mycolour,my_lglmoves,loc_board);
	legal_moves_local(opp_colour,opp_lglmoves,loc_board);
	my_pieces = my_lglmoves[0];
	opp_pieces = opp_lglmoves[0];
	if (my_pieces > opp_pieces)
	{
		m = (100 * my_pieces)/(my_pieces + opp_pieces);
	}else if(my_pieces < opp_pieces){
		m = -(100*opp_pieces)/(my_pieces+opp_pieces);
	}
	
	return m;

}

/**
 *   Rank i (i != 0) executes this code 
 *   ----------------------------------
 *   Called at the start of execution on all ranks except for rank 0.
 *   - run_worker should play minimax from its move(s) 
 *   - results should be send to Rank 0 for final selection of a move 
 */
int rank_no1(int my_rank){
	if(my_rank == 1){
		return 1;
	}
	return 0;
}

int rank_no2(int my_rank){
	if(my_rank == 2){
		return 1;
	}
	return 0;
}
int rank_no(int my_rank){
	if(my_rank == 3){
		return 1;
	}
	return 0;
}

void run_worker() {

	int size = 0;
	int my_rank = 0;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	
	// Broadcast colour
	int loc_mycolour = 0;
	MPI_Bcast(&loc_mycolour,1,MPI_INT,0,MPI_COMM_WORLD);
	
	int running = 1;
	//Broadcast running
	MPI_Bcast(&running,1,MPI_INT,0,MPI_COMM_WORLD);
	
	while (running == 1) {	
		
		// Broadcast board
		MPI_Bcast(board,BOARDSIZE,MPI_INT,0,MPI_COMM_WORLD);
		
		gen_move_worker(my_rank,size,loc_mycolour);
		
		//Broadcast running
		MPI_Bcast(&running,1,MPI_INT,0,MPI_COMM_WORLD); 
	}

}

void gen_move_worker(int my_rank, int size,int my_colour){
	int num_moves = 0;
	int *count = (int *) malloc(size*sizeof(int));
	int *displ = (int *) malloc(size*sizeof(int));
	memset(count, 0, size);
	memset(displ, 0, size);
	int *recvbuf_gather = NULL;
	int *moves_local = NULL;
	int *result_local = NULL;
	
		
	//broadcast num_move
	MPI_Bcast(&num_moves,1,MPI_INT,0,MPI_COMM_WORLD);
	if (num_moves == 0){
		return ;
	}else if (num_moves == 1)
	{
		return ;
	}
	
	//broadcast count
	MPI_Bcast(count,size,MPI_INT,0,MPI_COMM_WORLD);

	//broadcast displacement
	MPI_Bcast(displ,size,MPI_INT,0,MPI_COMM_WORLD);
		
	if(count[my_rank] > 0){
		recvbuf_gather = (int *) malloc((size*2)* sizeof(int));
		memset(recvbuf_gather, 0, num_moves);
		
		//Generate move
		int local_size = count[my_rank];
		moves_local = (int *) malloc(local_size*sizeof(int));
		memset(moves_local, 0, local_size);
		
		result_local = (int *) malloc(local_size*sizeof(int));
		memset(result_local, 0, local_size);

		//USING SCATTER
		MPI_Scatterv(result_local,count,displ,MPI_INT,moves_local,local_size,MPI_INT,0,MPI_COMM_WORLD);	

		
		int alpha = INT_MIN;
		int beta = INT_MAX;
		 
		int *mover = (int *) malloc(2*sizeof(int)); 
		
		int chosen_move;
		mover[0] = minmaxV3(board,my_colour,&chosen_move,alpha,beta,my_colour,moves_local,local_size,my_rank);
		
		
		int *bb = (int *) malloc(BOARDSIZE*sizeof(int));
		copy_board(bb,board);
		apply_move(bb,mover[0],my_colour);
		int val2 = eval_board(bb,my_colour);
		mover[1] = val2;
		
		MPI_Gather(mover,2,MPI_INT,recvbuf_gather,2,MPI_INT,0,MPI_COMM_WORLD);
		
		//free some stuff
		if (moves_local != NULL){free(moves_local);}
		if (result_local != NULL){free(result_local);}
		if (recvbuf_gather != NULL){free(recvbuf_gather);}
		if (mover != NULL){free(mover);}
		if (bb != NULL){free(bb);}

		
	}else if(count[my_rank]== 0){
		recvbuf_gather = (int *) malloc((size*2)* sizeof(int));
		memset(recvbuf_gather, 0, num_moves);

		int local_size = count[my_rank];
		moves_local = (int *) malloc(local_size*sizeof(int));
		memset(moves_local, 0, local_size);
	
		result_local = (int *) malloc(local_size*sizeof(int));
		memset(result_local, 0, local_size);
		
		int score = INT_MIN;
		int *mover = (int *) malloc(2*sizeof(int)); 
		mover[0] = -1;
		mover[1] = score;
		MPI_Scatterv(result_local,count,displ,MPI_INT,moves_local,local_size,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Gather(mover,2,MPI_INT,recvbuf_gather,2,MPI_INT,0,MPI_COMM_WORLD);
		
		//free stuff
		if (moves_local != NULL){free(moves_local);}
		if (result_local != NULL){free(result_local);}
		if (recvbuf_gather != NULL){free(recvbuf_gather);}
		if (mover != NULL){free(mover);}
	
	}
						
}

/**
 *  Rank 0 executes this code: 
 *  --------------------------
 *  Called when the next move should be generated 
 *  - gen_move_master should play minimax from its move(s)
 *  - the ranks may communicate during execution 
 *  - final results should be gathered at rank 0 for final selection of a move 
 */
void gen_move_master(char *move, int my_colour, FILE *fp) {
	int loc;

	/* generate move */
	//loc = random_strategy(my_colour, fp);
	loc = my_strategy(my_colour,fp);
	
	if (loc == -1) {
		strncpy(move, "pass\n", MOVEBUFSIZE);
	} else {
		/* apply move */
		
		get_move_string(loc, move);
		make_move(loc, my_colour, fp);	
	}
}

void apply_opp_move(char *move, int my_colour, FILE *fp) {
	int loc;
	if (strcmp(move, "pass\n") == 0) {
		return;
	}
	loc = get_loc(move);
	make_move(loc, opponent(my_colour, fp), fp);
}

void game_over() {
	
	free_board();
	MPI_Finalize();
}

void get_move_string(int loc, char *ms) {
	int row, col, new_loc;
	new_loc = loc - (9 + 2 * (loc / 10));
	row = new_loc / 8;
	col = new_loc % 8;
	ms[0] = row + '0';
	ms[1] = col + '0';
	ms[2] = '\n';
	ms[3] = 0;
}

int get_loc(char* movestring) {
	int row, col;
	/* movestring of form "xy", x = row and y = column */ 
	row = movestring[0] - '0'; 
	col = movestring[1] - '0'; 
	return (10 * (row + 1)) + col + 1;
}

void legal_moves(int player, int *moves, FILE *fp) {
	int move, i;
	moves[0] = 0;
	i = 0;
	for (move = 11; move <= 88; move++) {
		if (legalp(move, player, fp)) {
      i++;
      moves[i] = move; //storing the moves in the array
    }
	}
	moves[0] = i; //number of moves that can be made
}

int legalp(int move, int player, FILE *fp) {
	int i;
	if (!validp(move)) return 0;
	if (board[move] == EMPTY) {
		i = 0;
		while (i <= 7 && !would_flip(move, ALLDIRECTIONS[i], player, fp)) i++;
		if (i == 8) return 0; else return 1;
	}
	else return 0;
}

int validp(int move) {
	if ((move >= 11) && (move <= 88) && (move%10 >= 1) && (move%10 <= 8))
		return 1;
	else return 0;
}

int would_flip(int move, int dir, int player, FILE *fp) {
	int c;
	c = move + dir;
	if (board[c] == opponent(player, fp))
		return find_bracket_piece(c+dir, dir, player, fp);
	else return 0;
}

int find_bracket_piece(int square, int dir, int player, FILE *fp) {
	while (validp(square) && board[square] == opponent(player, fp)) square = square + dir;
	if (validp(square) && board[square] == player) return square;
	else return 0;
}

int opponent(int player, FILE *fp) {
	if (player == BLACK) return WHITE;
	if (player == WHITE) return BLACK;
	fprintf(fp, "illegal player\n"); return EMPTY;
}

int random_strategy(int my_colour, FILE *fp) {
	int r;
	int *moves = (int *) malloc(LEGALMOVSBUFSIZE * sizeof(int));
	memset(moves, 0, LEGALMOVSBUFSIZE);

	legal_moves(my_colour, moves, fp);
	if (moves[0] == 0) {
		return -1;
	}
	srand (time(NULL));
	r = moves[(rand() % moves[0]) + 1];//random move from move array
	return(r);
}

int my_strategy(int my_colour, FILE *fp) {

	
	//TODO RANK 0 gets to this method use
	int *moves = (int *) malloc(LEGALMOVSBUFSIZE*sizeof(int));
	memset(moves, 0, LEGALMOVSBUFSIZE);
	legal_moves(my_colour,moves,fp);
	

	int mov_size = moves[0]; 
	int *moving = (int *) malloc(mov_size*sizeof(int));
	memset(moving, 0, mov_size);

	for (int i = 0; i < mov_size; i++)
	{
		moving[i] = moves[i+1];
	}
	
	int num_moves = moves[0];
	int comm_sz;
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	
	MPI_Bcast(&num_moves,1,MPI_INT,0,MPI_COMM_WORLD);
	if (num_moves == 0)
	{
		return -1;
	}else if (num_moves == 1)
	{
		return moving[0];
	}
	
	int div = comm_sz;
	int displ[comm_sz];
	int *moves_local = NULL; 
	int count[comm_sz];
	init_zero(displ,count,comm_sz);

	int sz_of_recvbuf = comm_sz*2;
	int *recvbuf_gather = (int *) malloc(sz_of_recvbuf * sizeof(int));
	
	
	if (num_moves <= comm_sz)
	{
		
		for (int i = 0; i < num_moves; i++)
		{
			count[i] = count[i] + 1;
		}
		
	}else{	
		while (num_moves > 0)
		{
			int add_on = div%comm_sz;
			for (int i = 0; i < comm_sz; i++)
			{
				if (num_moves/div > 0)
				{
					if((i + add_on) >= comm_sz){
						add_on = 0;	
					}
					int update = i + add_on;
					count[update] = count[update] + num_moves/div;			
					num_moves = num_moves - (num_moves/div);
				} else{
					break;
				}
			}
			div = div - 1;
			if(div == 0){
				count[comm_sz-1] = count[comm_sz-1] + num_moves;
				num_moves = 0;
			}
		}
			
	}
	num_moves = moves[0];
	update_disp(displ,count,comm_sz);

	//broadcast Count 
	MPI_Bcast(count,comm_sz,MPI_INT,0,MPI_COMM_WORLD);
	
	//broadcast displ
	MPI_Bcast(displ,comm_sz,MPI_INT,0,MPI_COMM_WORLD);
	
	
	int local_size = count[0];
	moves_local = (int *) malloc(count[0]*sizeof(int));
	
	
	MPI_Scatterv(moving,count,displ,MPI_INT,moves_local,local_size,MPI_INT,0,MPI_COMM_WORLD);
	
	int alpha = INT_MIN;
	int beta = INT_MAX;
	
	
	int *mover = (int *) malloc(2*sizeof(int)); 
	int chosen_move;
	mover[0] = minmaxV3(board,my_colour,&chosen_move,alpha,beta,my_colour,moves_local,local_size,0);
	
	int *bb = (int *) malloc(BOARDSIZE*sizeof(int));
		copy_board(bb,board);
		apply_move(bb,mover[0],my_colour);
		int val2 = eval_board(bb,my_colour);
		mover[1] = val2;
	
	MPI_Gather(mover,2,MPI_INT,recvbuf_gather,2,MPI_INT,0,MPI_COMM_WORLD);
	
	int max = INT_MIN;
    int inx1 = 1;
    for (int i = 1; i < sz_of_recvbuf; i+=2)
    {
        if (recvbuf_gather[i] > max)
        {
            max = recvbuf_gather[i];
            inx1 = i;
        }
        
    }
	
	int rtn = recvbuf_gather[inx1 - 1]; //max score move  to make
	
	//free stuff 
	if (moves_local != NULL){free(moves_local);}
	if (moving != NULL){free(moving);}
	if (recvbuf_gather != NULL){free(recvbuf_gather);}
	if (moves != NULL){free(moves);}
	if (mover != NULL){free(mover);}
	if (bb != NULL){free(bb);}

	return rtn; 	
}

/*make disp array*/
void update_disp(int disp[], int count[], int size){
	
	disp[0] = 0;
	for (int i = 1; i < size; i++)
	{
		disp[i] = count[i-1] + disp[i-1];
	}
}

/*initialise array with zeros*/
void init_zero(int disp[],int count[],int size){
	for (int i = 0; i < size; i++)
	{
		disp[i] = 0;
		count[i] = 0;
	}
}



void copy_board(int *copy, int *origional){

	for (int  i = 0; i < BOARDSIZE; i++)
	{
		copy[i] = origional[i];
	}
}
void apply_move(int *loc_board,int mv,int player){
	int i;
	loc_board[mv] = player;
	for (i = 0; i <= 7; i++) make_flips_local(mv, ALLDIRECTIONS[i], player,loc_board);
}

int minmaxV3(int *loc_board, int player, int *chosen_move, int alpha, int beta,int loc_mycolour,int move[],int len,int my_rank){
	
	if (len == 0)
	{
		*chosen_move = -1;
	}else
	{
		int best_move_value = INT_MIN;
		int best_move = move[0];
		for (int i = 0; i < len; i++)
		{
			int *temp_board = (int *) malloc(BOARDSIZE*sizeof(int));
			copy_board(temp_board,loc_board);
            apply_move(temp_board,move[i],player);
			int *opp_leg_moves = (int *) malloc(LEGALMOVSBUFSIZE*sizeof(int));
			
			int opponent = opponent_local(loc_mycolour);
    		if (player == opponent) {
        		opponent = loc_mycolour;
    		}

			legal_moves_local(opponent,opp_leg_moves,temp_board);
			
			int num_move = opp_leg_moves[0];
			int *moving = (int *) malloc(num_move*sizeof(int));
			for (int i = 0; i < num_move; i++)
			{
				moving[i] = opp_leg_moves[i+1];
			}
			int value = minmax_value2(temp_board,player,opponent,0,alpha,beta,loc_mycolour,moving,num_move,my_rank);
			if(value > best_move_value){
				best_move_value = value;
				best_move = move[i];
			}
			if(temp_board != NULL){free(temp_board);}
			if(opp_leg_moves!= NULL){free(opp_leg_moves);}
		}
		*chosen_move = best_move;
	}
	
	return *chosen_move;
}


int minmax_value2(int *loc_board,int original_turn, int current_turn,int depth,int a,int b,int loc_mycolour,int mov[],int ln,int my_rank){
	
	if(depth == MAX_DEPTH){
		return eval_board(loc_board,loc_mycolour);
	}else if((count(WHITE,loc_board) + count(BLACK,loc_board)) == 64){
		//if positive i am winning if negative i am losing
		int opp_colour = opponent_local(loc_mycolour);
		int k = count(loc_mycolour,loc_board) - count(opp_colour,loc_board);
		return k;
	}
	int *lgl_moves = (int *) malloc(LEGALMOVSBUFSIZE*sizeof(int));
	legal_moves_local(current_turn,lgl_moves,loc_board);
	int num_moves = lgl_moves[0];
	int opponent = opponent_local(loc_mycolour);
	if (current_turn == opponent)
	{
		opponent = loc_mycolour;	
	}
	
	if (num_moves == 0)
	{
		return minmax_value2(loc_board,original_turn,opponent,depth+1,a,b,loc_mycolour,mov,ln,my_rank);
	}else
	{
		int best_move_value = INT_MIN;//max player
		int alpha = a;
		int beta = b;
		if (original_turn != current_turn)
		{
			best_move_value = INT_MAX;//minimising player
		}
	for (int i = 1; i <= num_moves; i++) 
	{
		int *temp_board = (int *) malloc(BOARDSIZE*sizeof(int));
		copy_board(temp_board,loc_board);//TODO use memcpy
		apply_move(temp_board,lgl_moves[i],current_turn);
		
		
	int value = minmax_value2(temp_board,original_turn,opponent,depth+1,alpha,beta,loc_mycolour,mov,ln,my_rank);
    	
		if (original_turn == current_turn) {
                if (value > best_move_value) {
                    best_move_value = value;
                }

                if (value > alpha) {
                    alpha = value;
                }
        } else {
                //MIN PLAYER
                if (value < best_move_value) {
                    best_move_value = value;
                }

                if (value < beta) {
                    beta = value;
                }
            }
			
			if (beta <= alpha) {
             
                break;
            }
				
	}
 		return best_move_value;
	}
	return -1;
}


int* make_move_local(int move, int player,int *loc_board) {
	int *rtn_board = (int *) malloc(BOARDSIZE*sizeof(int));
	
	for (int  i = 0; i < BOARDSIZE; i++)
	{
		rtn_board[i] = loc_board[i];
	}
	
	int i;
	loc_board[move] = player;
	for (i = 0; i <= 7; i++) make_flips_local(move, ALLDIRECTIONS[i], player,loc_board);
	return rtn_board;
}

void make_flips_local(int move, int dir, int player,int *loc_board) {
	int bracketer, c;
	bracketer = would_flip_local(move, dir, player,loc_board);
	if (bracketer) {
		c = move + dir;
		do {
			loc_board[c] = player;
			c = c + dir;
		} while (c != bracketer);
	}
}

int would_flip_local(int move, int dir, int player,int *loc_board) {
	int c;
	c = move + dir;
	if (loc_board[c] == opponent_local(player))
		return find_bracket_piece_local(c+dir, dir, player,loc_board);
	else return 0;
}

int find_bracket_piece_local(int square, int dir, int player, int *loc_board) {
	while (validp_local(square) && loc_board[square] == opponent_local(player)) square = square + dir;
	if (validp(square) && loc_board[square] == player) return square;
	else return 0;
}

int opponent_local(int player) {
	if (player == BLACK) {
		return WHITE;
	}else if (player == WHITE) {
		return BLACK;
	}
	return 0;
}



void make_move(int move, int player, FILE *fp) {
	int i;
	board[move] = player;
	for (i = 0; i <= 7; i++) make_flips(move, ALLDIRECTIONS[i], player, fp);
}

void make_flips(int move, int dir, int player, FILE *fp) {
	int bracketer, c;
	bracketer = would_flip(move, dir, player, fp);
	if (bracketer) {
		c = move + dir;
		do {
			board[c] = player;
			c = c + dir;
		} while (c != bracketer);
	}
}

void print_board(FILE *fp) {
	int row, col;
	fprintf(fp, "   1 2 3 4 5 6 7 8 [%c=%d %c=%d]\n",
		nameof(BLACK), count(BLACK, board), nameof(WHITE), count(WHITE, board));
	for (row = 1; row <= 8; row++) {
		fprintf(fp, "%d  ", row);
		for (col = 1; col <= 8; col++)
			fprintf(fp, "%c ", nameof(board[col + (10 * row)]));
		fprintf(fp, "\n");
	}
	fflush(fp);
}

char nameof(int piece) {
	assert(0 <= piece && piece < 5);
	return(piecenames[piece]);
}

int count(int player, int * board) {
	int i, cnt;
	cnt = 0;
	for (i = 1; i <= 88; i++)
		if (board[i] == player) cnt++;
	return cnt;
}





