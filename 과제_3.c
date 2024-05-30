#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define LEN_MIN  15   // 기차길이
#define LEN_MAX  50

#define STM_MIN  0    // 마동석체력
#define STM_MAX  5

#define PROB_MIN  10  // 확률
#define PROB_MAX  90

#define AGGRO_MIN  0  // 어그로범위
#define AGGRO_MAX  5

// 마동석이동방향
#define MOVE_LEFT  1
#define MOVE_STAY  0

// 좀비의공격대상
#define ATK_NONE  0
#define ATK_CITIZEN  1
#define ATK_DONGSEOK  2

// 마동석행동
#define ACTION_REST  0
#define ACTION_PROVOKE  1
#define ACTION_PULL  2

char train[50];

typedef struct basics {
	int end;
	int len;
	int prob;
	int count;
}basics;

typedef struct citizen {
	int c_move;
	int c_aggro;
	int c_location;
	int c_location_past;
}citizne;

typedef struct villaion {
	int v_aggro;
	int v_location;
	int v_location_past;
	int change_coin;
}villaion;

typedef struct zombie {
	int z_move;
	int z_location;
	int z_location_past;
	int z2m_attack_coin;
	int z2c_attack_coin;
	int z2v_attack_coin;
	int z_attack_coin;
}zombie;

typedef struct madoung {
	int stun_coin;
	int m_move_select;
	int m_action_select;
	int m_aggro;
	int m_location;
	int m_location_past;
	int stm;
}madoung;


int intro() { //인트로

	printf("====================================================================================================\n");
	printf("*****!!!!**!!!!!;!!!;;:;!!!;~~~,~~--~~,,-::~~~---,...,----,-!******************!;;!;!!!!!***!*******\n");
	printf("*****!!!!!**!!!!;;;;;;;*===;::~--~;!!!~,,~::~;*!;..,.-===!-:*****;~~**!::~:****!;;;!!!!!!***!*******\n");
	printf("****!!!!!!!!!!!;;;;;;;!$$#$!;:~~--!=##!,,,~:;$#$*,,,.~###*~!****!:  !!: ..,****!;;;;;!!!!*!!!*******\n");
	printf("******!!!!!!!;;;;;:;;;!#######$$==$$#$!,.,-;*@@#;-,.,,$==~,!***!.   .,~   ,****!;;;;;;!!!!!!********\n");
	printf("******!!;!!!;;;;;:::::!###$########$##!-,,-~$@@$*,. ,~;!=!~!***!     .~   ,****!;;;;;;;!!!!!!**!****\n");
	printf("*****!!!;;!!;;;;:;::::!#@@#$==$$=$#@##!--,,!@@@@$!,-,,###$*****!:    :,   ,****!;;!;:!;!!!!!********\n");
	printf("*****!!!;;;;;;;;;:::::!$#@#!!!!!!!=#@@*--,:###=###*:.-#$#$$****!, ,-      ,****!!;;;;!!!!!!**!******\n");
	printf("****!!!;;;;;;;;;;:::::!##@#==$#####@@@*~-;$@@$:*###$~:#$#$*****!. -- .-   ,****!;;;;;!!!!!**********\n");
	printf("****!!;;;;;;;;;:::::::!###$!$#@@$$####*~!#@@$*~,~=##!;#$#=:!***!:    ;~   ,****!;;;;!!!!!!**********\n");
	printf("****!!;;;;;;;;;::::::::;;::::;;;;;::::;:~;#$;,,.~~!$;;#@#$;!*****!!!:     -****!:~;;!!!!!***********\n");
	printf("**!*!!!;;;;;;;:::::::;=##=$$$##@@@@@@@#!~~:;:~~:;:;::;###$;!******;. ...  !****!~~:;!!!!!***********\n");
	printf("****!!!!!;;;;;:::::::;$$#$$#####@@@@@@@*:~;###;;;;;;:;$$$=:!******, ,!!;. -****!~-::!!!*********=***\n");
	printf("****!!!!!;;;;;::::::::******=####==*===!:~!@@@!;!;;;;;!!!;;!******- ,---. ~****!:-~:!!**************\n");
	printf("****!!!!!!;;;;::::::::::::::!###$!;;;;:;:~!@@@###$$$$$$$$=;!******;.     .!****!;~:;!;!;!******=**=*\n");
	printf("=*****!!!!!;;;;;:::::::::::~!###$;;::::;;~!#@####@@@####=$!!*******!!;;;!!*****!;;!*!*******!=*=====\n");
	printf("*=***!!!!!;!;;;:::::::~:::::!$$$=!!!:::;;:!#$$$####$$$###=;!********************!!!!*********=*=====\n");
	printf("*****!!!*!*;;;;;:::::::::~::::;;;!!!;::;;::;;;;!!;!;!;!!!;;;!;;::;!*************!!!!********==**====\n");
	printf("====================================================================================================\n");

	printf("\n");
	printf("\n");
	printf("\n");
	return 0;
}

int outro(int end) { //아웃트로
	if (end == 1) {
		printf("SUCCESS!\n");
		printf("citizen(s) escaped to the next train\n");
	}
	else {
		printf("GAME OVER!\n");
		printf("Citizen(s) has(have) been attacked by a zombie\n");
	}
	exit(1);
	return 0;
}

int train_length(int len) { //기차 길이
	while (1) {
		printf("train length(15~50)>> ");
		scanf_s("%d", &len);
		if (len >= LEN_MIN && len <= LEN_MAX) {
			break;
		}
	}
	return len;
}

int ma_stm(int stm) { //마동석 
	while (1) {
		printf("madongseokstamina(0~5)>> ");
		scanf_s("%d", &stm);
		if (stm >= STM_MIN && stm <= STM_MAX) {
			break;
		}
	}
	return stm;
}

int train_prob(int prob) { //확률
	while (1) {
		printf("percentile probability 'p'(10~90)>> ");
		scanf_s("%d", &prob);
		if (prob >= PROB_MIN && prob <= PROB_MAX) {
			break;
		}
	}
	return prob;
}

int train_inside(int len, int C, int Z, int M, int V, int stage) { //기차 내부 

	for (int i = 0; i < len; i++) {

		if (i == 0 || i == len - 1) {
			train[i] = '#'; // 양 끝을 '#'으로 설정
		}
		else if (stage == 2 && i == V) {
			train[i] = 'V'; // V의 위치
		}
		else if (i == C) {
			train[i] = 'C'; // C의 위치
		}
		else if (i == Z) {
			train[i] = 'Z'; // Z의 위치
		}
		else if (i == M) {
			train[i] = 'M'; // M의 위치
		}
		else {
			train[i] = ' '; // 나머지는 공백으로 설정
		}
	}
	train_state(len);
	printf("\n");
	return 0;
}

int train_state(int len) { //기차 상황 출력
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			for (int j = 0; j < len; j++) {
				printf("%c", train[j]);
			}
			printf("\n");
		}
		else {
			for (int j = 0; j < len; j++) {
				printf("#"); // 천장과 바닥을 '#'으로 채움
			}
			printf("\n");
		}
	}
	return 0;
}

citizne citizen_move(int c_move, int C, int c_aggro, int prob, int V, int change_coin);
citizne citizen_move(int c_move, int C, int c_aggro, int prob, int V, int change_coin) {
	citizne citizen;
	citizen.c_location_past = C;

	c_move = rand() % 100 + 1;
	citizen.c_move = c_move;
	if (change_coin == 1) {
		int tem = 0;
		tem = V;
		V = C;
		C = tem;
	}
	else if (c_move <= (100 - prob)) {
		C--;
		c_aggro++;
	}
	else c_aggro--;

	if (c_aggro < AGGRO_MIN) c_aggro = 0;
	if (c_aggro > AGGRO_MAX) c_aggro = 5;

	citizen.c_aggro = c_aggro;
	citizen.c_location = C;
	return citizen;
}

villaion villaion_move(int c_move, int V, int V_agro, int stage, int prob, int change_coin);
villaion villaion_move(int c_move, int V, int V_agro, int stage, int prob, int change_coin) {
	villaion villaion;
	villaion.v_location_past = V;
	
	if (stage == 2) {
		if (c_move <= (100 - prob)) {
			V--;
			V_agro++;
		}
		else V_agro--;

		if (V_agro < AGGRO_MIN) V_agro = 0;
		if (V_agro > AGGRO_MAX) V_agro = 5;
	}

	villaion.change_coin = change_coin;
	villaion.v_aggro = V_agro;
	villaion.v_location = V;
	return villaion;
}

zombie zombie_move(int z_move, int Z, int prob, int count, int C, int V, int M, int C_agro, int V_agro, int M_agro, int stun_coin, int z2c_attack_coin, int z2m_attack_coin, int z2v_attack_coin, int z_attack_coin);
zombie zombie_move(int z_move, int Z, int prob, int count, int C, int V, int M, int C_agro, int V_agro, int M_agro, int stun_coin, int z2c_attack_coin, int z2m_attack_coin, int z2v_attack_coin, int z_attack_coin) {
	zombie zombie;
	zombie.z_location_past = Z;
	
	if (count % 2 == 0) {
		if (stun_coin == 1) {
			printf("마동석의 붙들기에 의해 좀비는 행동 불가합니다.\n");
			stun_coin = 0;
		}
		else {
			z_move = rand() % 100 + 1;
			if (z_move <= prob) {
				if (M_agro > C_agro || M_agro > V_agro) {
					if (M == Z + 1) z2m_attack_coin += 1;
					else {
						Z++;
						if (M == Z + 1) z2m_attack_coin += 1;
					}
				}
				else {
					if (V == Z - 1) z2v_attack_coin += 1;
					if (C == Z - 1) z2c_attack_coin += 1;
					else {
						Z--;
						if (C == Z - 1) z2c_attack_coin += 1;
						if (V == Z - 1) z2v_attack_coin += 1;
					}
				}
				if (C + 1 == Z && M - 1 == Z) z_attack_coin += 1;
			}
		}
	}
	else printf("%d턴은 좀비의 행동 불가 턴입니다.\n", count);
	
	zombie.z2m_attack_coin = z2m_attack_coin;
	zombie.z2c_attack_coin = z2c_attack_coin;
	zombie.z2v_attack_coin = z2v_attack_coin;
	zombie.z_attack_coin = z_attack_coin;
	zombie.z_location = Z;

	return zombie;
}

madoung m_move_select(int move_select, int M, int m_aggro, int Z, int stm, int stun_coin);
madoung m_move_select(int move_select, int M, int m_aggro, int Z, int stm, int stun_coin) { //마동석 
	madoung madoung;
	madoung.m_location_past = M;

	while (1) {
		if (M - 1 == Z) {
			printf("madongseok move(0:stay)>> ");
			scanf_s("%d", &move_select);
			if (move_select != MOVE_STAY) continue;
		}
		else {
			printf("madongseok move(0:stay, 1:left)>> ");
			scanf_s("%d", &move_select);
		}
		if (move_select == MOVE_LEFT) {
			M -= 1;
			m_aggro += 1;
			break;
		}
		else if (move_select == MOVE_STAY) {
			m_aggro -= 1;
			break;
		}
	}
	if (m_aggro < AGGRO_MIN) m_aggro = 0;
	if (m_aggro > AGGRO_MAX) m_aggro = 5;

	madoung.m_location = M;
	madoung.m_aggro = m_aggro;
	madoung.m_move_select = move_select;
	madoung.stm = stm;
	madoung.stun_coin = stun_coin;

	return madoung;
}

int C_state(int c_location_past, int c_location, int c_aggro) { //시민과 좀비의 이동 결과 출력
	
	if (c_location_past != c_location) printf("citizen: %d -> %d (aggro: %d)\n", c_location_past, c_location, c_aggro);
	else printf("citizen: stay %d (aggro: %d)\n", c_location, c_aggro);
	return 0;
}

int V_state(int v_location_past, int v_location, int v_aggro, int stage) { //시민과 좀비의 이동 결과 출력
	
	if (stage == 2) {
		if (v_location != v_location_past) printf("villaion: %d -> %d (aggro: %d)\n", v_location_past, v_location, v_aggro);
		else printf("villaion: stay %d (aggro: %d)\n", v_location, v_aggro);
	}
	return 0;
}

int Z_state(int count, int z_location_past, int z_location) { //시민과 좀비의 이동 결과 출력
	
	if (count % 2 == 0) {
		if (z_location_past != z_location) printf("zombie: %d -> %d\n", z_location_past , z_location );
		else printf("zombie: stay %d\n", z_location);
	}
	else printf("zombie: stay %d\n", z_location);
		
	return 0;
}

int M_state(int m_location_past, int m_location, int m_aggro, int stm) { //마동석의 이동 결과 출력
	if (m_location_past != m_location) printf("madongseok: %d -> %d (aggro: %d, stamina: %d)\n", m_location_past, m_location, m_aggro, stm);
	else printf("madongseok: stay %d  (aggro: %d, stamina: %d)\n", m_location, m_aggro, stm);
	return 0;
}

int c_action(int C, int end, int stage) {
	if (C == 1) {
		if (stage == 3) {
			end = 1;
			outro(end);
		}
		printf("%d 스테이지 클리어! 다음 스테이지로 넘어갑니다...\n\n", stage);
		return 0;
	}
	else printf("citizen does nothing.\n");

	return 1;
}

int z_action(int z_attack_coin, int z2c_attack_coin, int z2m_attack_coin, int z2v_attack_coin, int C_agro, int M_agro, int V_agro, int stm, int end) {
	if (z_attack_coin == 1) {
		if (C_agro < M_agro || V_agro < M_agro) {
			stm -= 1;
			printf("Zomibe attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", C_agro, M_agro, stm + 1, stm);
			ma_dead_chack(stm, end);
		}
		else {
			end = 0;
			outro(end);
		}
	}
	else if (z2c_attack_coin == 1) {
		end = 0;
		outro(end);
	}
	else if (z2m_attack_coin == 1) {
		stm -= 1;
		printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n", stm + 1, stm);
		ma_dead_chack(stm, end);
	}
	else if (z2v_attack_coin == 1) {
		stm -= 1;
		printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n", stm + 1, stm);
		ma_dead_chack(stm, end);
	}
	else printf("zombie attacked nobody.\n");
	z2m_attack_coin = 0;
	z2c_attack_coin = 0;
	z_attack_coin = 0;

	return stm;
}
madoung m_action_select(int action_select, int stm, int M, int M_agro, int stun_coin, int Z, int prob, int end);
madoung m_action_select(int action_select, int stm, int M, int M_agro, int stun_coin, int Z, int prob, int end) {
	
	madoung madoung;

	while (1) {
		int pull = 0;
		if (M - 1 == Z) printf("madongseok action(0.rest, 1.provoke, 2. pull)>> ");
		else printf("madongseok action(0.rest, 1.provoke)>> ");

		scanf_s("%d", &action_select);
		if (action_select == ACTION_REST) {
			M_agro -= 1;
			stm += 1;
			if (M_agro < AGGRO_MIN) M_agro = AGGRO_MIN;
			if (M_agro > AGGRO_MAX) M_agro = AGGRO_MAX;
			if (stm > STM_MAX) stm = STM_MAX;
			if (stm == STM_MAX) printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro + 1, M_agro, stm, stm);
			else if (M_agro == AGGRO_MAX) printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro, M_agro, stm - 1, stm);
			else if (M_agro == AGGRO_MAX && stm == STM_MAX) printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro, M_agro, stm, stm);
			else printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro + 1, M_agro, stm - 1, stm);
			break;
		}
		else if (action_select == ACTION_PROVOKE) {
			printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", M, M_agro, AGGRO_MAX, stm);
			M_agro = AGGRO_MAX;
			break;
		}
		else if (action_select == ACTION_PULL) {
			M_agro += 2;
			stm -= 1;
			pull = rand() % 100 + 1;
			if (pull <= (100 - prob)) {
				printf("madongseok pulled zombie... Next turn, it can't move\n");
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro - 2, M_agro, stm + 1, stm);
				stun_coin += 1;
				madoung.stun_coin = stun_coin;
			}
			else {
				printf("madongseok failed to pull zombie\n");
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro - 2, M_agro, stm + 1, stm);
			}
			ma_dead_chack(stm, end);
			break;
		}
	}

	madoung.m_location_past = M;
	madoung.m_location = M;
	madoung.m_aggro = M_agro;
	madoung.stm = stm;
	
	return madoung;
}

villaion v_action(int C, int V, int change_coin, int v_aggro, int v_location, int v_location_past);
villaion v_action(int C, int V, int change_coin, int v_aggro, int v_location, int v_location_past) {
	villaion villaion;
	villaion.v_aggro = v_aggro;
	villaion.v_location = v_location;
	villaion.v_location_past = v_location_past;

	int v_move = 0;
	v_move = rand() % 100 + 1;
	if (v_move <= 30) {
		printf("발암, 성공...\n");
		change_coin++;
	}
	villaion.change_coin = change_coin;
	
	return villaion;
}


int ma_dead_chack(int stm, int end) {
	if (stm == STM_MIN) {
		end = 0;
		outro(end);
	}
	return 0;
}

int main() {
	citizne citizne;
	zombie zombie;
	madoung madoung;
	villaion villaion;
	basics basics;

	basics.end = 999;
	basics.len = 0;
	basics.prob = 0;
	madoung.stm = 0;

	intro();
	basics.len = train_length(basics.len);
	madoung.stm = ma_stm(madoung.stm);
	basics.prob = train_prob(basics.prob);


	for (int stage = 1; stage <= 3; stage++) {
		citizne.c_aggro = 1;
		madoung.m_aggro = 1;
		villaion.v_aggro = 1;

		citizne.c_location = basics.len - 6;
		zombie.z_location = basics.len - 3;
		madoung.m_location = basics.len - 2;
		villaion.v_location = basics.len - 5;

		citizne.c_location_past = citizne.c_location;
		zombie.z_location_past = zombie.z_location;
		madoung.m_location_past = madoung.m_location;
		villaion.v_location_past = villaion.v_location;

		madoung.stun_coin = 0;
		zombie.z2m_attack_coin = 0;
		zombie.z2c_attack_coin = 0;
		zombie.z2v_attack_coin = 0;
		zombie.z_attack_coin = 0;
		villaion.change_coin = 0;

		basics.count = 1;

		train_inside(basics.len, citizne.c_location, zombie.z_location, madoung.m_location, villaion.v_location, stage);

		while (1) {

			citizne.c_move = 0;
			zombie.z_move = 0;
			madoung.m_move_select = 0;
			madoung.m_action_select = 0;

			printf("\n");
			printf("\n");

			citizne = citizen_move(citizne.c_move, citizne.c_location, citizne.c_aggro, basics.prob, villaion.v_location, villaion.change_coin);
			villaion = villaion_move(citizne.c_move, villaion.v_location, villaion.v_aggro, stage, basics.prob, villaion.change_coin);
			zombie = zombie_move(zombie.z_move, zombie.z_location, basics.prob, basics.count, citizne.c_location, villaion.v_location, madoung.m_location, citizne.c_aggro, villaion.v_aggro, madoung.m_aggro, madoung.stun_coin, zombie.z2c_attack_coin,zombie.z2m_attack_coin,zombie.z2v_attack_coin,zombie.z_attack_coin);

			train_inside(basics.len, citizne.c_location, zombie.z_location, madoung.m_location, villaion.v_location, stage);
			
			C_state(citizne.c_location_past, citizne.c_location, citizne.c_aggro);
			V_state(villaion.v_location_past, villaion.v_location, villaion.v_aggro, stage);
			Z_state(basics.count, zombie.z_location_past, zombie.z_location);
			printf("\n");
			
			madoung = m_move_select(madoung.m_move_select, madoung.m_location, madoung.m_aggro, zombie.z_location, madoung.stm, madoung.stun_coin);
			train_inside(basics.len, citizne.c_location, zombie.z_location, madoung.m_location, villaion.v_location, stage);
			printf("\n");
			M_state(madoung.m_location_past, madoung.m_location, madoung.m_aggro, madoung.stm);

			printf("\n");

			if (c_action(citizne.c_location, basics.end, stage) == 0) break;
			if (stage == 2) villaion = v_action(citizne.c_location, villaion.v_location, villaion.change_coin, villaion.v_aggro, villaion.v_location, villaion.v_location_past);
			z_action(zombie.z_attack_coin, zombie.z2c_attack_coin, zombie.z2m_attack_coin, zombie.z2v_attack_coin, citizne.c_aggro, madoung.m_aggro, villaion.v_aggro, madoung.stm, basics.end);
			madoung = m_action_select(madoung.m_action_select, madoung.stm,madoung.m_location,madoung.m_aggro,madoung.stun_coin,zombie.z_location,basics.prob, basics.end);
			basics.count++;
		}
	}
	basics.end = 1;
	outro(basics.end);

	return 0;
}