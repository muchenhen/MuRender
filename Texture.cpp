#include "Texture.h"

//bmp�ļ��ṹ����
//ref:http://www.cnblogs.com/wainiwann/p/7086844.html
//LoadBmp����ο�֪���û�"����"cobraԴ��
//�˺������ܿ�ʹ��windows.h����LoadImage()��������

bool TextureL::LoadBmp(const std::string& filename)
{
	std::ifstream is(filename, std::ios_base::binary);
	if (!is) return false;
	unsigned char buf[54];//bmp�ļ�ͷ+��Ϣͷ����54�ֽ�
	is.read((char*)buf, sizeof(buf));
	width = *(int*)&buf[18];//18�ֽ����4���ֽڴ��bmp���biWidth
	height = abs(*(int*)&buf[22]);//λͼ�߶�ֵ,�����Ϊ��ֵ����ŵ��õ�ͼ��Ϊ��ֵʱ�����ͼ��biHeight
	int bits = buf[28]; //buf[28]biBitCountֵΪ1,4,8,16,24��32,ÿ����ռ��λ��,��ʾ24λλͼ,32λλͼ��
	int rowcountBytes = ((width*bits + 31) >> 5) << 2;//����Ϊÿ��4�ֽڶ���,���빫ʽ(rowbits+31)/32*4
	int count = rowcountBytes * height;
	unsigned char* tmp = new unsigned char[count];//bmp���ݻ���
	is.read((char*)tmp, count);
	int bytes = bits / 8, cnts = 0;
	int bof =((width*bits) >> 3) & 3;
	int paddings = (4-bof)==4?0:(4-bof) ;//paddings,ÿ��4�ֽڶ������������ֽ�
	data = new UINT[width*height];
	for (int i = 0; i < width*height;i++)//��������Texture
	{
		//ֻ����24λ��32λ���ֳ���λͼ��ʽ,����alphaͨ��
		//BGRA to RGBA or BGR to RGB
		//ʹ��windows����RGB��,����ʹ��Math_L���Զ�����ɫת������ColorToUINT()
		data[i] = RGB(tmp[cnts + 2], tmp[cnts + 1], tmp[cnts + 0]);
		cnts += bytes;
		if (cnts + paddings >= rowcountBytes && (cnts + paddings) % rowcountBytes == 0)
			cnts += paddings;//skip paddings
	}
	delete[] tmp;
	return true;
}
//����ӳ��
//��D3D12���飬����wrap,border color,clamp,mirror4�ֲ�ͬ��ӳ��Ѱַ��ʽ
//�ڴ���3Dģ�͵�����ʱ,��Ҫʹ��������˹���,�ݲ�ʵ�ֹ��˹���.
UINT TextureL::SampColor(Eigen::Vector4f& tc)
{
	float u = Math_L::Clamp(tc[0], 0.f, 1.f);
	float v = Math_L::Clamp(tc[1], 0.f, 1.f);

	int u1 = (int)(std::round(u * (width)));
	int v1 = (int)(std::round(v * (height)));

	return data[u1 + v1 * width];
}