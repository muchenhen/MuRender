#include "Texture.h"

//bmp文件结构介绍
//ref:http://www.cnblogs.com/wainiwann/p/7086844.html
//LoadBmp代码参考知乎用户"金条"cobra源码
//此函数功能可使用windows.h内置LoadImage()函数代替

bool TextureL::LoadBmp(const std::string& filename)
{
	std::ifstream is(filename, std::ios_base::binary);
	if (!is) return false;
	unsigned char buf[54];//bmp文件头+信息头缓存54字节
	is.read((char*)buf, sizeof(buf));
	width = *(int*)&buf[18];//18字节起的4个字节存放bmp宽度biWidth
	height = abs(*(int*)&buf[22]);//位图高度值,大多数为正值，存放倒置的图像，为负值时存放正图像biHeight
	int bits = buf[28]; //buf[28]biBitCount值为1,4,8,16,24或32,每像素占用位数,表示24位位图,32位位图等
	int rowcountBytes = ((width*bits + 31) >> 5) << 2;//必须为每行4字节对齐,对齐公式(rowbits+31)/32*4
	int count = rowcountBytes * height;
	unsigned char* tmp = new unsigned char[count];//bmp数据缓存
	is.read((char*)tmp, count);
	int bytes = bits / 8, cnts = 0;
	int bof =((width*bits) >> 3) & 3;
	int paddings = (4-bof)==4?0:(4-bof) ;//paddings,每行4字节对齐所需的填充字节
	data = new UINT[width*height];
	for (int i = 0; i < width*height;i++)//数据载入Texture
	{
		//只处理24位或32位二种常见位图形式,丢弃alpha通道
		//BGRA to RGBA or BGR to RGB
		//使用windows内置RGB宏,或者使用Math_L中自定义颜色转换函数ColorToUINT()
		data[i] = RGB(tmp[cnts + 2], tmp[cnts + 1], tmp[cnts + 0]);
		cnts += bytes;
		if (cnts + paddings >= rowcountBytes && (cnts + paddings) % rowcountBytes == 0)
			cnts += paddings;//skip paddings
	}
	delete[] tmp;
	return true;
}
//纹理映射
//据D3D12龙书，共有wrap,border color,clamp,mirror4种不同的映射寻址方式
//在处理3D模型的缩放时,需要使用纹理过滤功能,暂不实现过滤功能.
UINT TextureL::SampColor(Eigen::Vector4f& tc)
{
	float u = Math_L::Clamp(tc[0], 0.f, 1.f);
	float v = Math_L::Clamp(tc[1], 0.f, 1.f);

	int u1 = (int)(std::round(u * (width)));
	int v1 = (int)(std::round(v * (height)));

	return data[u1 + v1 * width];
}