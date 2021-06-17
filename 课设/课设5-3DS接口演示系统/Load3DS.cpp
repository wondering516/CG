// Load3DS.cpp: implementation of the CLoad3DS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Load3DS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoad3DS::CLoad3DS()
{
	NumOfMaterial=1;
	NumOfObject=0;
	CMaterial3d defaultMat;
	defaultMat.HasTexture=FALSE;
	strcpy(defaultMat.MaterialName, "Default");
	defaultMat.DifColor[0]=192;
	defaultMat.DifColor[1]=192;
	defaultMat.DifColor[2]=192;
	pMaterial.push_back(defaultMat);
}

CLoad3DS::~CLoad3DS()
{
	while(pMaterial.size()!=0)
		pMaterial.pop_back();
	for(int i=0;i<NumOfObject;i++)
	{
		delete [] pObject[i]->pFace;
		delete [] pObject[i]->pVertex;
		delete [] pObject[i]->pFaceNormal;
		delete [] pObject[i]->pVertNormal;
		delete [] pObject[i]->pTexVertex;
		delete pObject[i];
	}
	pObject.clear();
}

BOOL CLoad3DS::Load3DSFile(CString FileName)
{
	if(!cFile.Open(FileName,CFile::modeRead))// 打开3DS文件
	{
		char err[128];
		sprintf(err, "打开3DS文件：%s 失败!", FileName);
		MessageBox(NULL, err,"读取错误",MB_OK|MB_ICONINFORMATION);
		return FALSE;	
	}	
	CChunk CurrentChunk=ReadChunkHead();		// 首先读出文件的主块
	if(CurrentChunk.ID==MAIN3DS)				// 判断是否为3DS文件
	{
		ReadFileData(CurrentChunk.Length-6);
	}
	else
	{
		char err[128];
		sprintf(err, "%s 不是3DS文件", FileName);
		MessageBox(NULL, err, "读取错误", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	CalculateNormal();
	cFile.Close();
	return TRUE;
}

CChunk CLoad3DS::ReadChunkHead()//读取块信息
{
	CChunk chunk;
	cFile.Read(&chunk.ID,2);		// 读出块的ID，占2个字节
	cFile.Read(&chunk.Length,4);    // 读出块的长度，占4个字节
	return chunk;
}

void CLoad3DS::ReadFileData(long EndPos)
{
	DWORD  CurrentPos=cFile.GetPosition();//当前文件的指针位置						
	while(CurrentPos<EndPos)
	{
		CChunk CurrentChunk=ReadChunkHead();
		switch(CurrentChunk.ID)
		{
		case EDIT3DS://不做任何处理接着读下一个数据块
			break;
		case EDIT_MATERIAL: //材质信息
			NumOfMaterial++;
			CurrentPos=cFile.GetPosition();
			ReadMaterialData(CurrentPos+CurrentChunk.Length-6);//跳过块的偏移量
			break;
		case EDIT_OBJECT:
			NumOfObject++;
			CurrentPos=cFile.GetPosition();
			ReadObjectData(CurrentPos+CurrentChunk.Length-6);
			break;
		default:
			cFile.Seek(CurrentChunk.Length-6,CFile::current);//移动文件指针
		}
		CurrentPos=cFile.GetPosition();
	}
}

void CLoad3DS::ReadObjectData(long EndPos)//读取模型
{
	int i,Total,MatIndex=0;
	CObjects *Obj=new CObjects;//定义一个CObjects对象用来存放读出的EDIT_OBJECT块数据
	ReadString(Obj->ObjName);//首先读出对象的名称,文件指针跳过该字符串
	long CurrentPos=cFile.GetPosition();	
	while(cFile.GetPosition()<EndPos)
	{
		CChunk CurrentChunk=ReadChunkHead();
		switch(CurrentChunk.ID)
		{
		case TRI_MESH:
			break;
		case TRI_VERTEX://读出顶点信息
			Obj->NumOfVertex=ReadWord();//Obj中的顶点数目，占1个字
			Obj->pVertex=new CP3[Obj->NumOfVertex];//分配数组空间
			//3dmax坐标系方向与本设计定义的坐标系不同，需要调换y、z坐标值，3dmax坐标系 x向右，y轴向内，z轴向上
			for(i=0;i<Obj->NumOfVertex;i++)
			{
				Obj->pVertex[i].x=ReadFloat();
				Obj->pVertex[i].z=-ReadFloat();
				Obj->pVertex[i].y=ReadFloat();
			}
			break;
		case TRI_FACE://读出面信息
			Obj->NumOfFace=ReadWord();//Obj中的面数目
			Obj->pFace=new CFace[Obj->NumOfFace];//分配空间
			// 读取面索引值(第4个值为3dMAX使用的参数，舍弃)
			for(i=0;i<Obj->NumOfFace;i++)
			{
				Obj->pFace[i].p[0]=ReadWord();
				Obj->pFace[i].p[1]=ReadWord();
				Obj->pFace[i].p[2]=ReadWord();
				cFile.Seek(2,CFile::current);
			}
			break;
		case FACE_MAT://读出面的材质信息
			char MaterialName[255];
			ReadString(MaterialName);//读出面使用的材质名称
			Total=ReadWord();//读出使用该材质的面数
			for(i=1; i<NumOfMaterial; i++)//遍历所有材质找到该材质在材质数组中的索引号
			{
				if(strcmp(MaterialName, pMaterial[i].MaterialName) == 0)
				{
					MatIndex=i;
					break;
				}
			}
			while(Total>0)//遍历所有使用该材质的面
			{	
				Obj->pFace[ReadWord()].MaterialIndex = MatIndex;//将材质的索引写入对应面的信息中
				Total--;
			}
			break;
		case OBJ_UV://读出纹理坐标信息
			Obj->NumOfTexVertex = ReadWord();//读出Obj中的纹理坐标数目
			Obj->pTexVertex = new CT2[Obj->NumOfTexVertex];//分配空间
			for(i=0;i<Obj->NumOfTexVertex;i++)
			{
				Obj->pTexVertex[i].u=ReadFloat();
				Obj->pTexVertex[i].v=ReadFloat();
			}
			break;
		default:
			cFile.Seek(CurrentChunk.Length-6,CFile::current);
			break;
		}
		CurrentPos=cFile.GetPosition();;		// 得到当前文件指针的位置
	}
	Obj->pFaceNormal=NULL;
	Obj->pVertNormal=NULL;
	pObject.push_back(Obj);			// 把读好的对象数据压栈
}

void CLoad3DS::ReadMaterialData(long EndPos)
{
	CMaterial3d *Mat=new CMaterial3d;// 定义一个OBJECT对象用来存放读出的EDIT_MATRIAL块数据
	long CurrentPos=cFile.GetPosition();	
	while(CurrentPos<EndPos)
	{
		CChunk CurrentChunk=ReadChunkHead();
		switch(CurrentChunk.ID)
		{
		case MAT_NAME:
			ReadString(Mat->MaterialName);
			break;
		case MAT_DIF:
			break;
		case COLOR_BYTE:
			Mat->DifColor[0]=ReadByte();
			Mat->DifColor[1]=ReadByte();
			Mat->DifColor[2]=ReadByte();
			break;
		/*case MAT_SPE:
			break;*/
		case MAT_MAP:
			break;
		case MAP_NAME:
			ReadString(Mat->TextureName);
			Mat->HasTexture=TRUE;
			break;
		default:
			cFile.Seek(CurrentChunk.Length-6,CFile::current);
		}
		CurrentPos=cFile.GetPosition();//得到当前文件指针的位置
	}
	pMaterial.push_back(* Mat);
	delete Mat;
}

void CLoad3DS::ReadString(char Str[])
{
	char c;
	for(int Length=0;Length<255;Length++)
	{
		cFile.Read(&c,1);
		Str[Length]=c;		
		if(c=='\0')
			return ;
	}
}

BYTE CLoad3DS::ReadByte()
{
	BYTE Byte;
	cFile.Read(&Byte,1);
	return Byte;
}

WORD CLoad3DS::ReadWord()
{
	WORD Word;
	cFile.Read(&Word,sizeof(WORD));
	return Word;
}

float CLoad3DS::ReadFloat()
{
	float Float;
	cFile.Read(&Float,sizeof(float));
	return Float;
}

void CLoad3DS::CalculateNormal()
{
	if (NumOfObject<=0)//如果没有3ds对象则直接返回
		return;
	for(int i=0;i<NumOfObject;i++)
	{
		if(pObject[i]->pFaceNormal==NULL)
		{
			pObject[i]->pFaceNormal=new CVector[pObject[i]->NumOfFace];//为面的法矢量分配空间
		}
		if(pObject[i]->pVertNormal==NULL)
		{
			pObject[i]->pVertNormal=new CVector[pObject[i]->NumOfVertex];//为点的法矢量分配空间
		}
		// 计算对象中每个面的法矢量
		for(int j=0;j<pObject[i]->NumOfFace;j++)
		{	
			CVector v1,v2,Normal;
			CFace CurrentFace=pObject[i]->pFace[j];
			CP3 Point0=pObject[i]->pVertex[CurrentFace.p[0]];//三角形的第一个顶点
			CP3 Point1=pObject[i]->pVertex[CurrentFace.p[1]];//三角形的第二个顶点
		    CP3 Point2=pObject[i]->pVertex[CurrentFace.p[2]];//三角形的第三个顶点
			v1=CVector(Point0,Point1);//计算当前三角形面片的边矢量
			v2=CVector(Point1,Point2);//计算当前三角形面片的边矢量
			Normal=v1*v2;//计算当前三角形面片的法矢量
			Normal=Normal.Unit();//单位化法矢量
			pObject[i]->pFaceNormal[j]=Normal;//保存单位化的面法矢量
		}									
		for (int Index=0;Index<pObject[i]->NumOfVertex;Index++)//遍历所有顶点
		{
			CVector SumVector;//累加矢量	
			int Counter=0;//面片计数
			for (int nFace=0;nFace<pObject[i]->NumOfFace;nFace++)//遍历包含该顶点的面
			{
				if(pObject[i]->pFace[nFace].p[0]==Index || 
				   pObject[i]->pFace[nFace].p[1]==Index || 
				   pObject[i]->pFace[nFace].p[2]==Index)
				{
					SumVector=SumVector+pObject[i]->pFaceNormal[nFace];
					Counter++;
				}
			} 
			SumVector=SumVector/float(Counter);
			pObject[i]->pVertNormal[Index]=SumVector.Unit();//计算单位化的顶点平均法矢量
		}
	}
}

