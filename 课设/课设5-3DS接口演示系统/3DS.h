#ifndef _3DS_H_
#define _3DS_H_

//3DS文件结构是层次结构，定义使用到的ChunkID
#define MAIN3DS	0x4D4D							//主块
#define		EDIT3DS 0x3D3D						//3D编辑块
#define			EDIT_MATERIAL 0xAFFF			//材质编辑块
#define				MAT_NAME  0xA000			//材质名称
#define				MAT_AMB   0xA010		    //材质环境光颜色
#define				MAT_DIF   0xA020		    //材质漫反射光颜色
#define				MAT_SPE   0xA030		    //材质镜面反射光颜色
#define			    MAT_MAP   0xA200	        //材质的纹理
#define					MAP_NAME 0xA300		    //纹理的名称
#define			EDIT_OBJECT   0x4000			//对象块
#define				TRI_MESH 0x4100				//三角形网格
#define					TRI_VERTEX 0x4110		//三角形顶点
#define					TRI_FACE   0x4120		//三角形表面
#define						FACE_MAT    0x4130	//表面的材质
#define						FACE_SMOOTH 0x4150	//表面光滑信息（本设计未使用）
#define					OBJ_UV     0x4140		//纹理坐标
#define					OBJ_LOCAL  0x4160       //转换矩阵
#define		PRIM_KEY 0xB000					    //关键帧块（本设计未使用）
#define	COLOR_BYTE  0x0011//颜色块
#define COLOR_FLOAT 0x0010

#endif