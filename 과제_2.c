//���� c�� z�� m ��ġ �����ͷ� �ޱ� 


#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

// �Ķ����
#define LEN_MIN  15   // ��������
#define LEN_MAX  50

#define STM_MIN  0    // ������ü��
#define STM_MAX  5

#define PROB_MIN  10  // Ȯ��
#define PROB_MAX  90

#define AGGRO_MIN  0  // ��׷ι���
#define AGGRO_MAX  5

// �������̵�����
#define MOVE_LEFT  1
#define MOVE_STAY  0

// �����ǰ��ݴ��
#define ATK_NONE  0
#define ATK_CITIZEN  1
#define ATK_DONGSEOK  2

// �������ൿ
#define ACTION_REST  0
#define ACTION_PROVOKE  1
#define ACTION_PULL  2

char train[50];

int C_agro = 1;
int M_agro = 1;

int z2c_attack_coin = 0;
int z2m_attack_coin = 0;
int z_attack_coin = 0;
int stun_coin = 0;


int intro() { //��Ʈ��

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

int outro(int end) { //�ƿ�Ʈ��
	if (end == 1) {
		printf("SUCCESS!\n");
		printf("citizen(s) escaped to the next train\n");
	}
	else if(end == 2) {
		printf("GAME OVER!\n");
		printf("Madongsack(s) has(have) been attacked by a zombie\n");
	}
	else {
		printf("GAME OVER!\n");
		printf("Citizen(s) has(have) been attacked by a zombie\n");
	}
	exit(1);
	return 0;
}

int train_length(int len) { //���� ����
	while (1) {
		printf("train length(15~50)>> ");
		scanf_s("%d", &len);
		if (len >= LEN_MIN && len <= LEN_MAX) {
			break;
		}
	}
	return len;
}

int ma_stm(int stm) { //������ 
	while (1) {
		printf("madongseokstamina(0~5)>> ");
		scanf_s("%d", &stm);
		if (stm >= STM_MIN && stm <= STM_MAX) {
			break;
		}
	}
	return stm;
}

int train_prob(int prob) { //Ȯ��
	while (1) {
		printf("percentile probability 'p'(10~90)>> ");
		scanf_s("%d", &prob);
		if (prob >= PROB_MIN && prob <= PROB_MAX) {
			break;
		}
	}
	return prob;
}

int train_inside(int len, int C, int Z, int M) { //���� ���� 

	for (int i = 0; i < len; i++) {

		if (i == 0 || i == len - 1) {
			train[i] = '#'; // �� ���� '#'���� ����
		}
		else if (i == C) {
			train[i] = 'C'; // C�� ��ġ
		}
		else if (i == Z) {
			train[i] = 'Z'; // Z�� ��ġ
		}
		else if (i == M) {
			train[i] = 'M'; // M�� ��ġ
		}
		else {
			train[i] = ' '; // �������� �������� ����
		}
	}
	train_state(len);
	printf("\n");
	return 0;
}

int train_state(int len) { //���� ��Ȳ ���
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			for (int j = 0; j < len; j++) {
				printf("%c", train[j]);
			}
			printf("\n");
		}
		else {
			for (int j = 0; j < len; j++) {
				printf("#"); // õ��� �ٴ��� '#'���� ä��
			}
			printf("\n");
		}
	}
	return 0;
}

int C_state(int past_C, int C) { //�ùΰ� ������ �̵� ��� ���
	if (past_C != C) {
		printf("citizen: %d -> %d (aggro: %d)\n", past_C, C, C_agro);
	}
	else {
		printf("citizen: stay %d (aggro: %d)\n", C, C_agro);
	}
	return 0;
}


int Z_state(int past_Z, int Z, int count) { //�ùΰ� ������ �̵� ��� ���
	if (count % 2 == 0) {
		if (past_Z != Z) {
			printf("zombie: %d -> %d\n", past_Z, Z);
		}
		else {
			printf("zombie: stay %d\n", Z);
		}
	}
	else {
		printf("zombie: stay %d (cannot move)\n", Z);
	}
	return 0;
}

int m_move_select(int move_select, int M, int Z) { //������ 
	while (1) {
		if (M - 1 == Z) {
			printf("madongseok move(0:stay)>> ");
			scanf_s("%d", &move_select);
			if (move_select != MOVE_STAY) {
				continue;
			}
		}
		else {
			printf("madongseok move(0:stay, 1:left)>> ");
			scanf_s("%d", &move_select);
		}
		if (move_select == MOVE_LEFT) {
			M -= 1;
			M_agro += 1;
			break;
		}
		else if (move_select == MOVE_STAY) {
			M_agro -= 1;
			break;
		}
	}
	if (M_agro < AGGRO_MIN) {
		M_agro = 0;
	}
	if (M_agro > AGGRO_MAX) {
		M_agro = 5;
	}

	return M;
}

int M_state(int past_M, int M, int stm) { //�������� �̵� ��� ���
	if (past_M != M) {
		printf("madongseok: %d -> %d (aggro: %d, stamina: %d)\n", past_M, M, M_agro, stm);
	}
	else {
		printf("madongseok: stay %d  (aggro: %d, stamina: %d)\n", M, M_agro, stm);
	}
	return 0;
}

int citizen_move(int c_move, int C, int prob) {
	c_move = rand() % 100 + 1;
	if (c_move <= (100 - prob)) {
		C--;
		C_agro++;
	}
	else {
		C_agro--;
	}

	if (C_agro < AGGRO_MIN) {
		C_agro = 0;
	}
	if (C_agro > AGGRO_MAX) {
		C_agro = 5;
	}

	return C;
}

int zombie_move(int z_move, int Z, int prob, int count, int C, int M) {
	if (count % 2 == 0) {
		if (stun_coin == 1) {
			printf("�������� �ٵ�⿡ ���� ����� �ൿ �Ұ��մϴ�.\n");
			stun_coin = 0;
		}
		else {
			z_move = rand() % 100 + 1;
			if (z_move <= prob) {
				if (M_agro > C_agro) {
					if (M == Z + 1) {
						z2m_attack_coin += 1;
					}
					else {
						Z++;
						if (M == Z + 1) {
							z2m_attack_coin += 1;
						}
					}
				}
				else {
					if (C == Z - 1) {
						z2c_attack_coin += 1;
					}
					else {
						Z--;
						if (C == Z - 1) {
							z2c_attack_coin += 1;
						}
					}
				}
				if (C + 1 == Z && M - 1 == Z) {
					z_attack_coin += 1;
				}
			}
		}
	}
	else {
		printf("%d���� ������ �ൿ �Ұ� ���Դϴ�.\n", count);
	}
	return Z;
}

int c_action(int C, int end) {
	if (C == 1) {
		end = 1;
		outro(end);
	}
	else {
		printf("citizen does nothing.\n");
	}

	return 1;
}

int z_action(int stm, int end) {
	if (z_attack_coin == 1) {
		if (C_agro < M_agro) {
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
	else {
		printf("zombie attacked nobody.\n");
	}
	z2m_attack_coin = 0;
	z2c_attack_coin = 0;
	z_attack_coin = 0;
	return stm;
}

int ma_dead_chack(int stm, int end) {
	if (stm == STM_MIN) {
		end = 2;
		outro(end);
	}
	return 0;
}

int m_action_select(int action_select, int stm, int M, int Z, int prob, int end) {
	while (1) {
		int pull = 0;
		if (M - 1 == Z) {
			printf("madongseok action(0.rest, 1.provoke, 2. pull)>> ");
		}
		else {
			printf("madongseok action(0.rest, 1.provoke)>> ");
		}

		scanf_s("%d", &action_select);
		if (action_select == ACTION_REST) {
			M_agro -= 1;
			stm += 1;
			if (M_agro < AGGRO_MIN) {
				M_agro = AGGRO_MIN;
			}
			if (M_agro > AGGRO_MAX) {
				M_agro = AGGRO_MAX;
			}
			if (stm > STM_MAX) {
				stm = STM_MAX;
			}
			if (stm == STM_MAX) {
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro + 1, M_agro, stm, stm);
			}
			else if (M_agro == AGGRO_MAX) {
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro, M_agro, stm - 1, stm);
			}
			else if (M_agro == AGGRO_MAX && stm == STM_MAX) {
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro, M_agro, stm, stm);
			}
			else {
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro + 1, M_agro, stm - 1, stm);
			}
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
			}
			else {
				printf("madongseok failed to pull zombie\n");
				printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, M_agro - 2, M_agro, stm + 1, stm);
			}
			ma_dead_chack(stm, end);
			break;
		}
	}
	return stm;
}

int main(void) {
	int len = 0;
	int prob = 0;
	int stm = 0;

	int end = 999;


	intro();
	len = train_length(len);
	stm = ma_stm(stm);
	prob = train_prob(prob);


		int C = len - 6;
		int Z = len - 3;
		int M = len - 2;
		int count = 1;
		while (1) { //�ùΰ� ������ �̵� ����

			printf("\n=================================%d��=================================\n", count);

			printf("\n---------------------�̵� ������---------------------\n");
			int move = 0;
			int c_move = 0;
			int z_move = 0;

			int move_select = 0;
			int action_select = 0;
			int past_Z = Z;
			int past_C = C;
			int past_M = M;

			printf("\n");
			printf("\n");

			C = citizen_move(c_move, C, prob);
			Z = zombie_move(z_move, Z, prob, count, C, M);

			train_inside(len, C, Z, M);

			C_state(past_C, C);
			Z_state(past_Z, Z, count);
			printf("\n");

			M = m_move_select(move_select, M, Z);
			train_inside(len, C, Z, M);
			M_state(past_M, M, stm);
			printf("\n---------------------�ൿ ������---------------------\n");
			c_action(C, end);


			stm = z_action(stm, end);
			printf("\n");
			stm = m_action_select(action_select, stm, M, Z, prob, end);
			printf("\n");
			count++;
		}
	end = 1;
	outro(end);
}