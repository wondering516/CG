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
	if(!cFile.Open(FileName,CFile::modeRead))// ��3DS�ļ�
	{
		char err[128];
		sprintf(err, "��3DS�ļ���%s ʧ��!", FileName);
		MessageBox(NULL, err,"��ȡ����",MB_OK|MB_ICONINFORMATION);
		return FALSE;	
	}	
	CChunk CurrentChunk=ReadChunkHead();		// ���ȶ����ļ�������
	if(CurrentChunk.ID==MAIN3DS)				// �ж��Ƿ�Ϊ3DS�ļ�
	{
		ReadFileData(CurrentChunk.Length-6);
	}
	else
	{
		char err[128];
		sprintf(err, "%s ����3DS�ļ�", FileName);
		MessageBox(NULL, err, "��ȡ����", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	CalculateNormal();
	cFile.Close();
	return TRUE;
}

CChunk CLoad3DS::ReadChunkHead()//��ȡ����Ϣ
{
	CChunk chunk;
	cFile.Read(&chunk.ID,2);		// �������ID��ռ2���ֽ�
	cFile.Read(&chunk.Length,4);    // ������ĳ��ȣ�ռ4���ֽ�
	return chunk;
}

void CLoad3DS::ReadFileData(long EndPos)
{
	DWORD  CurrentPos=cFile.GetPosition();//��ǰ�ļ���ָ��λ��						
	while(CurrentPos<EndPos)
	{
		CChunk CurrentChunk=ReadChunkHead();
		switch(CurrentChunk.ID)
		{
		case EDIT3DS://�����κδ�����Ŷ���һ�����ݿ�
			break;
		case EDIT_MATERIAL: //������Ϣ
			NumOfMaterial++;
			CurrentPos=cFile.GetPosition();
			ReadMaterialData(CurrentPos+CurrentChunk.Length-6);//�������ƫ����
			break;
		case EDIT_OBJECT:
			NumOfObject++;
			CurrentPos=cFile.GetPosition();
			ReadObjectData(CurrentPos+CurrentChunk.Length-6);
			break;
		default:
			cFile.Seek(CurrentChunk.Length-6,CFile::current);//�ƶ��ļ�ָ��
		}
		CurrentPos=cFile.GetPosition();
	}
}

void CLoad3DS::ReadObjectData(long EndPos)//��ȡģ��
{
	int i,Total,MatIndex=0;
	CObjects *Obj=new CObjects;//����һ��CObjects����������Ŷ�����EDIT_OBJECT������
	ReadString(Obj->ObjName);//���ȶ������������,�ļ�ָ���������ַ���
	long CurrentPos=cFile.GetPosition();	
	while(cFile.GetPosition()<EndPos)
	{
		CChunk CurrentChunk=ReadChunkHead();
		switch(CurrentChunk.ID)
		{
		case TRI_MESH:
			break;
		case TRI_VERTEX://����������Ϣ
			Obj->NumOfVertex=ReadWord();//Obj�еĶ�����Ŀ��ռ1����
			Obj->pVertex=new CP3[Obj->NumOfVertex];//��������ռ�
			//3dmax����ϵ�����뱾��ƶ��������ϵ��ͬ����Ҫ����y��z����ֵ��3dmax����ϵ x���ң�y�����ڣ�z������
			for(i=0;i<Obj->NumOfVertex;i++)
			{
				Obj->pVertex[i].x=ReadFloat();
				Obj->pVertex[i].z=-ReadFloat();
				Obj->pVertex[i].y=ReadFloat();
			}
			break;
		case TRI_FACE://��������Ϣ
			Obj->NumOfFace=ReadWord();//Obj�е�����Ŀ
			Obj->pFace=new CFace[Obj->NumOfFace];//����ռ�
			// ��ȡ������ֵ(��4��ֵΪ3dMAXʹ�õĲ���������)
			for(i=0;i<Obj->NumOfFace;i++)
			{
				Obj->pFace[i].p[0]=ReadWord();
				Obj->pFace[i].p[1]=ReadWord();
				Obj->pFace[i].p[2]=ReadWord();
				cFile.Seek(2,CFile::current);
			}
			break;
		case FACE_MAT://������Ĳ�����Ϣ
			char MaterialName[255];
			ReadString(MaterialName);//������ʹ�õĲ�������
			Total=ReadWord();//����ʹ�øò��ʵ�����
			for(i=1; i<NumOfMaterial; i++)//�������в����ҵ��ò����ڲ��������е�������
			{
				if(strcmp(MaterialName, pMaterial[i].MaterialName) == 0)
				{
					MatIndex=i;
					break;
				}
			}
			while(Total>0)//��������ʹ�øò��ʵ���
			{	
				Obj->pFace[ReadWord()].MaterialIndex = MatIndex;//�����ʵ�����д���Ӧ�����Ϣ��
				Total--;
			}
			break;
		case OBJ_UV://��������������Ϣ
			Obj->NumOfTexVertex = ReadWord();//����Obj�е�����������Ŀ
			Obj->pTexVertex = new CT2[Obj->NumOfTexVertex];//����ռ�
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
		CurrentPos=cFile.GetPosition();;		// �õ���ǰ�ļ�ָ���λ��
	}
	Obj->pFaceNormal=NULL;
	Obj->pVertNormal=NULL;
	pObject.push_back(Obj);			// �Ѷ��õĶ�������ѹջ
}

void CLoad3DS::ReadMaterialData(long EndPos)
{
	CMaterial3d *Mat=new CMaterial3d;// ����һ��OBJECT����������Ŷ�����EDIT_MATRIAL������
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
		CurrentPos=cFile.GetPosition();//�õ���ǰ�ļ�ָ���λ��
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
	if (NumOfObject<=0)//���û��3ds������ֱ�ӷ���
		return;
	for(int i=0;i<NumOfObject;i++)
	{
		if(pObject[i]->pFaceNormal==NULL)
		{
			pObject[i]->pFaceNormal=new CVector[pObject[i]->NumOfFace];//Ϊ��ķ�ʸ������ռ�
		}
		if(pObject[i]->pVertNormal==NULL)
		{
			pObject[i]->pVertNormal=new CVector[pObject[i]->NumOfVertex];//Ϊ��ķ�ʸ������ռ�
		}
		// ���������ÿ����ķ�ʸ��
		for(int j=0;j<pObject[i]->NumOfFace;j++)
		{	
			CVector v1,v2,Normal;
			CFace CurrentFace=pObject[i]->pFace[j];
			CP3 Point0=pObject[i]->pVertex[CurrentFace.p[0]];//�����εĵ�һ������
			CP3 Point1=pObject[i]->pVertex[CurrentFace.p[1]];//�����εĵڶ�������
		    CP3 Point2=pObject[i]->pVertex[CurrentFace.p[2]];//�����εĵ���������
			v1=CVector(Point0,Point1);//���㵱ǰ��������Ƭ�ı�ʸ��
			v2=CVector(Point1,Point2);//���㵱ǰ��������Ƭ�ı�ʸ��
			Normal=v1*v2;//���㵱ǰ��������Ƭ�ķ�ʸ��
			Normal=Normal.Unit();//��λ����ʸ��
			pObject[i]->pFaceNormal[j]=Normal;//���浥λ�����淨ʸ��
		}									
		for (int Index=0;Index<pObject[i]->NumOfVertex;Index++)//�������ж���
		{
			CVector SumVector;//�ۼ�ʸ��	
			int Counter=0;//��Ƭ����
			for (int nFace=0;nFace<pObject[i]->NumOfFace;nFace++)//���������ö������
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
			pObject[i]->pVertNormal[Index]=SumVector.Unit();//���㵥λ���Ķ���ƽ����ʸ��
		}
	}
}

