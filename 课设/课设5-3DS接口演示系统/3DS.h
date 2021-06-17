#ifndef _3DS_H_
#define _3DS_H_

//3DS�ļ��ṹ�ǲ�νṹ������ʹ�õ���ChunkID
#define MAIN3DS	0x4D4D							//����
#define		EDIT3DS 0x3D3D						//3D�༭��
#define			EDIT_MATERIAL 0xAFFF			//���ʱ༭��
#define				MAT_NAME  0xA000			//��������
#define				MAT_AMB   0xA010		    //���ʻ�������ɫ
#define				MAT_DIF   0xA020		    //�������������ɫ
#define				MAT_SPE   0xA030		    //���ʾ��淴�����ɫ
#define			    MAT_MAP   0xA200	        //���ʵ�����
#define					MAP_NAME 0xA300		    //���������
#define			EDIT_OBJECT   0x4000			//�����
#define				TRI_MESH 0x4100				//����������
#define					TRI_VERTEX 0x4110		//�����ζ���
#define					TRI_FACE   0x4120		//�����α���
#define						FACE_MAT    0x4130	//����Ĳ���
#define						FACE_SMOOTH 0x4150	//����⻬��Ϣ�������δʹ�ã�
#define					OBJ_UV     0x4140		//��������
#define					OBJ_LOCAL  0x4160       //ת������
#define		PRIM_KEY 0xB000					    //�ؼ�֡�飨�����δʹ�ã�
#define	COLOR_BYTE  0x0011//��ɫ��
#define COLOR_FLOAT 0x0010

#endif