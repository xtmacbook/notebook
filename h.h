
#ifndef  _geo_mipmapping_
#define  _geo_mipmapping_

#include <smartPointer.h>
#include <BaseObject.h>
#include <texture.h>
#include <flashBuffer.h>
#include <vertex.h>
#include <shader.h>
//08/04/2020

class Camera;

#define TRN_NUM_TILES 5

struct HeightData
{
	float *      heightData_ = nullptr;
	unsigned int xSize_ = 0;
	unsigned int ySize_ = 0;

	~HeightData()
	{
		if (heightData_)
		{
			delete[] heightData_;
		}
	}
};

struct TerrainBlock
{
	~TerrainBlock()
	{
		if (errList_)
		{
			delete[] errList_;
			errList_ = nullptr;
		}
		if (dSeqList_)
		{
			delete[]dSeqList_;
			dSeqList_ = nullptr;
		}
	}

	unsigned int xIndex_ = 0;
	unsigned int yIndex_ = 0;
	unsigned char displayLevel_ = 0;//best level to display
	V3f         center_ = V3f(FLT_MAX, FLT_MAX, FLT_MAX);
	float*		errList_ = nullptr;
	float*		dSeqList_ = nullptr;

	bool        visiable_ = false;
};

enum ETILE_TYPES
{
	LOWEST_TILE = 0,	//sand, dirt, etc.
	LOW_TILE,			//grass
	HIGH_TILE,			//mountainside
	HIGHEST_TILE		//tip of mountain
};


struct Strn_texture_regions
{
	int m_iLowHeight;			//lowest possible height (0%)
	int m_iOptimalHeight;		//optimal height (100%)
	int m_iHighHeight;			//highest possible height (0%)
};

class GeoMipMapping :public Base::BaseObject
{
public:

	GeoMipMapping();
	virtual ~GeoMipMapping();

	bool	initGometry();
	bool	initBlock();

	bool	initHeightMap(const char*heightFile);
	void	initConstant(const Camera*camera, float vh);

	bool	initHeightTexture(unsigned int,unsigned int);

	void    updateGeometry(const Camera*);

	void    render();

	Shader*    initShader();

	inline unsigned char getTrueHeightAtPoint(int iX, int iY)
	{
		return heightData_.heightData_[iY * heightData_.xSize_ + iX];
	}
private:
	
	bool		 initBlockErrList(TerrainBlock*);
	void		 initBlockCenter(TerrainBlock*);
	void		 updateBlockState(TerrainBlock*,const Camera*);
	void		 getBlockStart(TerrainBlock*, float*x, float *y);
	void         updateBlockGeometry(TerrainBlock*, Vertex*vertexData, unsigned int * indicesData);
	
	void						SetupBottomEdgeNormally(TerrainBlock *block, unsigned int global_index_offset,unsigned int *);
	void						SetupRightEdgeNormally(TerrainBlock *block, unsigned int global_index_offset, unsigned int *);
	void						SetupTopEdgeNormally(TerrainBlock *block, unsigned int global_index_offset, unsigned int *);
	void						SetupLeftEdgeNormally(TerrainBlock *block, unsigned int global_index_offset, unsigned int *);

	void						SetupBottomEdgeFixGap(TerrainBlock *block, TerrainBlock *bottom_neighbor, unsigned int global_index_offset, unsigned int *);
	void						SetupRightEdgeFixGap(TerrainBlock *block, TerrainBlock *right_neighbor, unsigned int global_index_offset, unsigned int *);
	void						SetupTopEdgeFixGap(TerrainBlock *block, TerrainBlock *top_neighbor, unsigned int global_index_offset, unsigned int *);
	void						SetupLeftEdgeFixGap(TerrainBlock *block, TerrainBlock *left_neighbor, unsigned int global_index_offset, unsigned int *);

	void getTexCoords(unsigned int imageW, unsigned int imageH, unsigned int* x, unsigned int* y);
	float regionPercent(int tileType, unsigned char ucHeight);
	unsigned char interpolateHeight(int x, int y, float fHeightToTexRatioX, float fHeightToTexRatioY);

	Texture_Sp				texture_;
	TerrainBlock**			blocks_ = nullptr;
	
	unsigned int			xsize_ = 0;
	unsigned int			ysize_ = 0;

	FlashBuffer<Vertex>*	vertexs_ = nullptr; //for vertexs
	FlashBuffer<unsigned int>* indices_ = nullptr; //for indices
	Shader*					shader_ = nullptr;

	HeightData				heightData_;

	unsigned int			blocksXCount_ = 0;
	unsigned int			blocksYCount_ = 0;

	unsigned int			maxLevel_ = 0;
	unsigned int			num_vertexs_ = 0;
	unsigned int			num_indices_ = 0;

	unsigned int			numTexture_ = 0;
	Strn_texture_regions	regions_[TRN_NUM_TILES];
	float					maxPixelError_;
	float					C_;

	float					start_X_ = 0.0f; //for all terrain
	float					start_Y_ = 0.0f;

	GLuint					vao_ = 0;
};

typedef Base::SmartPointer<GeoMipMapping> GeoMipMapping_SP;

#endif
