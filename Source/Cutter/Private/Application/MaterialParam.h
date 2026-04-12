#pragma once

/*マテリアル操作用パラメータ定義*/
class MaterialParam
{
public:
	/*アルファ値操作(別途マテリアルにパラメータ追加、変更が必要)*/
	static FName AlphaControl;
};

FName MaterialParam::AlphaControl = "AlphaControl";