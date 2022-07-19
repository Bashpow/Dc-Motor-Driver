#include "user_commands.h"
#include "shell.h"
#include "shellio.h"
extern Shell_command_t shell_cmd_root;

//用户头文件包含
#include "rtt_cmd.h"

//变量定义

//函数声明
static void Gyroscope_Data(char * arg);
static void Pid_Show(char * arg);
static void Pid_Set(char * arg);
static void Pid2_Show(char * arg);
static void Pid2_Set(char * arg);
static void Pid3_Show(char * arg);
static void Pid3_Set(char * arg);

//用户命令初始化
void User_Commands_Init(void)
{
	//变量初始化

    //操作系统命令注册
    Rtt_Cmd_Init();

	//用户命令注册
	Shell_Register_Command("gyroscope-data" , Gyroscope_Data);
	Shell_Register_Command("pid-show" , Pid_Show);
	Shell_Register_Command("pid-set" , Pid_Set);
	Shell_Register_Command("pid2-show" , Pid2_Show);
	Shell_Register_Command("pid2-set" , Pid2_Set);
	Shell_Register_Command("pid3-show" , Pid3_Show);
	Shell_Register_Command("pid3-set" , Pid3_Set);
}

static void Gyroscope_Data(char * arg)
{
	if(0)
	{
		// shell_print("roll:%.2f, pitch:%.2f, yaw:%.2f", Get_Gyro_Data()->angle.roll_x, Get_Gyro_Data()->angle.pitch_y, Get_Gyro_Data()->angle.yaw_z);
	}
	else
	{
		shell_print("Gyroscope offline!");
	}
	shell_print("\r\n");
}

float easy_pid_p, easy_pid_i, easy_pid_d;
static void Pid_Show(char * arg)
{
	shell_print("p:%.6f\ti:%.6f\td:%.6f\r\n", easy_pid_p, easy_pid_i, easy_pid_d);
}
static void Pid_Set(char * arg)
{
	/*
	char * argv[4];
	int argc = Shell_Split_String((char*)arg,argv,4);
	shell_print("get %d parameter\r\n", argc);
	for(int i = 0 ; i < argc ; ++i)
		shell_print("argv[%d]: \"%s\"\r\n",i,argv[i]);
	*/
	char * argv[3];
	int argc = Shell_Split_String(arg, argv, 3);
	switch(argv[1][0])
	{
		case 'p':
			easy_pid_p = String_To_Float(argv[2]);
			shell_print("EASY PID p set %.6f", easy_pid_p);
			break;

		case 'i':
			easy_pid_i = String_To_Float(argv[2]);
			shell_print("EASY PID i set %.6f", easy_pid_i);
			break;

		case 'd':
			easy_pid_d = String_To_Float(argv[2]);
			shell_print("EASY PID d set %.6f", easy_pid_d);
			break;

		default:
			shell_print("Error!");
			break;
	}
	shell_print("\r\n");
}

float easy_pid2_p, easy_pid2_i, easy_pid2_d;
static void Pid2_Show(char * arg)
{
	shell_print("p:%.6f\ti:%.6f\td:%.6f\r\n", easy_pid2_p, easy_pid2_i, easy_pid2_d);
}
static void Pid2_Set(char * arg)
{
	char * argv[3];
	int argc = Shell_Split_String(arg, argv, 3);
	switch(argv[1][0])
	{
		case 'p':
			easy_pid2_p = String_To_Float(argv[2]);
			shell_print("EASY PID2 p set %.6f", easy_pid2_p);
			break;

		case 'i':
			easy_pid2_i = String_To_Float(argv[2]);
			shell_print("EASY PID2 i set %.6f", easy_pid2_i);
			break;

		case 'd':
			easy_pid2_d = String_To_Float(argv[2]);
			shell_print("EASY PID2 d set %.6f", easy_pid2_d);
			break;

		default:
			shell_print("Error!");
			break;
	}
	shell_print("\r\n");
}

float easy_pid3_p, easy_pid3_i, easy_pid3_d;
static void Pid3_Show(char * arg)
{
	shell_print("p:%.6f\ti:%.6f\td:%.6f\r\n", easy_pid3_p, easy_pid3_i, easy_pid3_d);
}
static void Pid3_Set(char * arg)
{
	char * argv[3];
	int argc = Shell_Split_String(arg, argv, 3);
	switch(argv[1][0])
	{
		case 'p':
			easy_pid3_p = String_To_Float(argv[2]);
			shell_print("EASY PID3 p set %.6f", easy_pid3_p);
			break;

		case 'i':
			easy_pid3_i = String_To_Float(argv[2]);
			shell_print("EASY PID3 i set %.6f", easy_pid3_i);
			break;

		case 'd':
			easy_pid3_d = String_To_Float(argv[2]);
			shell_print("EASY PID3 d set %.6f", easy_pid3_d);
			break;

		default:
			shell_print("Error!");
			break;
	}
	shell_print("\r\n");
}
