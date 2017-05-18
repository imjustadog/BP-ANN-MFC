#include "stdafx.h"
#include "BPANN.h"

void clsANN::init_weight() //初始化权重为-1~1之间的随机数，初始化权重变化量都是0
{
	int i, j;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		for (j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].dweight[j] = 0;
			NodeOutput[i].weight[j] = (rand() * 2.0 / RAND_MAX) - 1; //rand()生成0~RAND_MAX之间的随机数
		}
	}
	for (i = 0; i < HIDE_DIM; i++)
	{
		for (j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].dweight[j] = 0;
			NodeHide[i].weight[j] = (rand() * 2.0 / RAND_MAX) - 1;//rand()生成0~RAND_MAX之间的随机数
			NodeHide[i].sum_dw = 0;
		}
	}
}

void clsANN::findmaxmin() //找输入输出的最大值和最小值，计算最大值和最小值之差
{
	int i, j;
	for (i = 0; i < INPUT_DIM; i++)
	{
		TrainDataIn[i].TrainData_Max = TrainDataIn[i].TrainData[0];
		TrainDataIn[i].TrainData_Min = TrainDataIn[i].TrainData[0];
		for (j = 0; j < SAMPLE_NUM; j++)
		{
			if (TrainDataIn[i].TrainData[j] > TrainDataIn[i].TrainData_Max) TrainDataIn[i].TrainData_Max = TrainDataIn[i].TrainData[j];
			if (TrainDataIn[i].TrainData[j] < TrainDataIn[i].TrainData_Min) TrainDataIn[i].TrainData_Min = TrainDataIn[i].TrainData[j];
		}
		TrainDataIn[i].TrainData_MaxMin = TrainDataIn[i].TrainData_Max - TrainDataIn[i].TrainData_Min + 1;
	}

	for (i = 0; i < OUTPUT_DIM; i++)
	{
		TrainDataOut[i].TrainData_Max = TrainDataOut[i].TrainData[0];
		TrainDataOut[i].TrainData_Min = TrainDataOut[i].TrainData[0];
		for (j = 0; j < SAMPLE_NUM; j++)
		{
			if (TrainDataOut[i].TrainData[j] > TrainDataOut[i].TrainData_Max) TrainDataOut[i].TrainData_Max = TrainDataOut[i].TrainData[j];
			if (TrainDataOut[i].TrainData[j] < TrainDataOut[i].TrainData_Min) TrainDataOut[i].TrainData_Min = TrainDataOut[i].TrainData[j];
		}
		TrainDataOut[i].TrainData_MaxMin = TrainDataOut[i].TrainData_Max - TrainDataOut[i].TrainData_Min + 1;
	}
}

void clsANN::input_normalization(int num_sample) //归一化输入
{
	int i, j;
	for (i = 0; i < INPUT_DIM; i++)
	{
		for (j = 0; j < num_sample; j++)
		{
			TrainDataIn[i].TrainData[j] = (TrainDataIn[i].TrainData[j] - TrainDataIn[i].TrainData_Min + 1) / TrainDataIn[i].TrainData_MaxMin;
		}
	}
}

void clsANN::output_normalization(int num_sample) //归一化输出
{
	int i, j;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		for (j = 0; j < num_sample; j++)
		{
			TrainDataOut[i].TrainData[j] = (TrainDataOut[i].TrainData[j] - TrainDataOut[i].TrainData_Min + 1) / TrainDataOut[i].TrainData_MaxMin;
		}
	}
}

void clsANN::output_anti_normalization(int num_sample) //网络输出去归一化
{
	int i;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		ResultOut[i][num_sample] = NodeOutput[i].Output_y  * TrainDataOut[i].TrainData_MaxMin + TrainDataOut[i].TrainData_Min - 1;
	}
}

void clsANN::calc_output(int group_sample) //计算网络输出
{
	int i, j;
	for (i = 0; i < HIDE_DIM; i++) //计算隐含层节点输出，隐含层激活函数使用sigmoid函数
	{
		NodeHide[i].Output_x = 0;
		for (j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].Output_x += NodeHide[i].weight[j] * TrainDataIn[j].TrainData[group_sample];
		}
		NodeHide[i].Output_y = 1.0 / (1.0 + exp((-1.0) * NodeHide[i].Output_x));
		NodeHide[i].Output_dy = 1.0 * NodeHide[i].Output_y * (1.0 - NodeHide[i].Output_y);
	}
	for (i = 0; i < OUTPUT_DIM; i++) //计算输出层节点输出，输出层激活函数直接使用y = x
	{
		NodeOutput[i].Output_x = 0;
		for (j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].Output_x += NodeOutput[i].weight[j] * NodeHide[j].Output_y;
		}
		NodeOutput[i].Output_y = NodeOutput[i].Output_x; //1.0 / (1.0 + exp((-1.0) * NodeOutput[i].Output_x)); //
		NodeOutput[i].Output_dy = 1.0 * NodeOutput[i].Output_y * (1.0 - NodeOutput[i].Output_y);
		error += fabs((TrainDataOut[i].TrainData[group_sample] - NodeOutput[i].Output_y) / TrainDataOut[i].TrainData[group_sample]);
		//if (error > error_max) error_max = error;
	}
}

void clsANN::calc_dweight(int group_sample) //反向传播法计算权重调整值
{
	int i, j;
	double delta;
	for (i = 0; i < OUTPUT_DIM; i++) //按照公式计算输出层节点的权重调整值并累计每个隐层节点输出的delta*weight之和用于计算隐层节点的权重调整值
	{
		delta = (TrainDataOut[i].TrainData[group_sample] - NodeOutput[i].Output_y);//* NodeOutput[i].Output_dy;
		for (j = 0; j <HIDE_DIM; j++)
		{
			NodeHide[j].sum_dw += delta * NodeOutput[i].weight[j];
			NodeOutput[i].dweight[j] += rate * delta * NodeHide[j].Output_y;
		}
	}

	for (i = 0; i < HIDE_DIM; i++) //按照公式计算隐层节点的权重调整值
	{
		delta = NodeHide[i].sum_dw * NodeHide[i].Output_dy;
		NodeHide[i].sum_dw = 0;
		for (j = 0; j <INPUT_DIM; j++)
		{
			NodeHide[i].dweight[j] += rate * delta * TrainDataIn[j].TrainData[group_sample];
		}
	}
}

void clsANN::adjust_weight() //调整权重
{
	int i, j;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		for (j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].weight[j] += NodeOutput[i].dweight[j];
			NodeOutput[i].dweight[j] = 0;
		}
	}

	for (i = 0; i < HIDE_DIM; i++)
	{
		for (j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].weight[j] += NodeHide[i].dweight[j];
			NodeHide[i].dweight[j] = 0;
		}
	}
}

void clsANN::do_train(double r, double e, int max_iteration) //训练函数
{
	_cprintf("train\r\n");
	double error_pre = 0;
	int i, j;
	srand((unsigned)time(NULL));
	rate = r;
	error_limit = e;
	init_weight(); //初始化权重
	findmaxmin(); //找最大最小值
	input_normalization(SAMPLE_NUM); //归一化输入
	output_normalization(SAMPLE_NUM); //归一化输出
	for (i = 0; 0 < max_iteration; i++) //正常来说应该设置最大迭代次数，但是因为误差一直达不到要求所以最大迭代次数设成了无穷
	{
		error = 0; //正常来说应该判断误差最大值，但用误差最大值训练了一晚上也没降到百分之五一下最后只好用平均值
		for (j = 0; j < SAMPLE_NUM; j++) //每一组样本都计算输出，使用BP算法计算出误差调整值进行累计
		{
			calc_output(j);
			calc_dweight(j);
		}
		error = error / SAMPLE_NUM; //误差平均值
		if (error <= error_limit) break; //平均值小于百分之五训练停止
		if (error > 1) //自己脑补了一个调整学习率的改进方法
		{
			if (error > error_pre || error_pre - error < 0.005) //如果训练完后误差反而变大了或者误差下降的太慢则调整学习率
			rate = r * (rand() * 1.0 / RAND_MAX + 0.5); //随机生成一个范围在1~1.5的因子乘以设定的学习率，即随机的增大或者减小学习率
		}
		else rate = r; //如果误差已经降到1以下就不再折腾，安心训练
		error_pre = error;
		_cprintf("%d %lf\r\n", i, error);
		adjust_weight(); //调整一波权重
	}
	is_trained = true; //训练过了，标志置1
}

void clsANN::calc_result() //将训练样本带回到网络中计算结果进行验证
{
	_cprintf("simulate\r\n");
	if (is_trained == false)
	{
		findmaxmin();
		input_normalization(SAMPLE_NUM);
	}
	for (int j = 0; j < SAMPLE_NUM; j++)
	{
		calc_output(j);
		output_anti_normalization(j);
		if (is_trained == false) _cprintf("real:%lf result:%lf\r\n", TrainDataOut[0].TrainData[j], ResultOut[0][j]);
		else _cprintf("real:%lf result:%lf\r\n", TrainDataOut[0].TrainData[j] * TrainDataOut[0].TrainData_MaxMin + TrainDataOut[0].TrainData_Min - 1, ResultOut[0][j]);
	}
}

double clsANN::calc_test(double x[]) //输入两个数，进行验证
{
	double y;
	for (int i = 0; i < INPUT_DIM; i++)
	{
		x[i] = (x[i] - TrainDataIn[i].TrainData_Min + 1) / (TrainDataIn[i].TrainData_Max - TrainDataIn[i].TrainData_Min + 1);
	}
	for (int i = 0; i < HIDE_DIM; i++)
	{
		NodeHide[i].Output_x = 0;
		for (int j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].Output_x += NodeHide[i].weight[j] * x[j];
		}
		NodeHide[i].Output_y = 1.0 / (1.0 + exp((-1.0) * NodeHide[i].Output_x));
		NodeHide[i].Output_dy = 1.0 * NodeHide[i].Output_y * (1.0 - NodeHide[i].Output_y);
	}
	for (int i = 0; i < OUTPUT_DIM; i++)
	{
		NodeOutput[i].Output_x = 0;
		for (int j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].Output_x += NodeOutput[i].weight[j] * NodeHide[j].Output_y;
		}
		NodeOutput[i].Output_y = NodeOutput[i].Output_x; //1.0 / (1.0 + exp((-1.0) * NodeOutput[i].Output_x)); 
	}
	y = NodeOutput[0].Output_y  * (TrainDataOut[0].TrainData_Max - TrainDataOut[0].TrainData_Min + 1) + TrainDataOut[0].TrainData_Min - 1;
	return y;
}