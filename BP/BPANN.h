#pragma once

#include <math.h>
#include <time.h>
#include <conio.h>

using namespace std;

#define INPUT_DIM 2 //输入变量个数
#define OUTPUT_DIM 1 //输出变量个数
#define HIDE_DIM 45 //隐含节点个数

#define SAMPLE_NUM 1000 //训练样本总数


struct structNode
{
	double weight[50]; //当前节点对每一个输入的权重
	double dweight[50]; //每次训练后每个权重需要改变的量
	double delta; //计算每层节点权重改变量时用到的系数
	double sum_dw; //计算隐含层节点权重改变量时用到的系数
	double Output_x;//当前节点输入之和
	double Output_y; //当前节点输出
	double Output_dy; //当前节点输出的导数
};
typedef struct structNode strctNode; //节点结构体

struct structData
{
	double TrainData[SAMPLE_NUM]; //数据：原始的→归一化后的
	double TrainData_Max; //数据最大值，归一化用
	double TrainData_Min; //数据最小值，归一化用
	double TrainData_MaxMin; //数据最大值和最小值之差，归一化用
};
typedef struct structData strctData; //数据（输入或输出）结构体

class clsANN
{
public:
	double error_limit; //误差限，误差小于它则停止训练
	double error; //当前误差
	double error_max; //当前最大误差
	double rate; //学习率

	bool is_trained; //仿真验证的时候用的，是不是已经归一化过了
	
	strctNode NodeOutput[OUTPUT_DIM]; //输出节点
	strctNode NodeHide[HIDE_DIM]; //隐含层节点
	                                                    //输入节点直接就是输入
	strctData TrainDataIn[INPUT_DIM]; //储存输入训练数据
	strctData TrainDataOut[OUTPUT_DIM]; //储存输出训练数据
	double ResultOut[OUTPUT_DIM][SAMPLE_NUM]; //储存神经网络计算出的结果

	void do_train(double rate, double error_limit, int max_iteration); //训练函数
	void calc_result(); //仿真验证函数
	double calc_test(double x[]); //测试函数

	void init_weight(); //使用随机数初始化权重
	                             
	void findmaxmin(); //找最大最小值
	void input_normalization(int num_sample); //输入数据归一化
	void output_normalization(int num_sample); //输出数据归一化
	void output_anti_normalization(int num_sample); //神经网络输出反归一化

	void calc_output(int group_sample); //计算当前输出
	void calc_dweight(int group_sample); //计算当前权重需要改变的量
	void adjust_weight(); //调整权重
};

