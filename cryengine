IRenderMesh:
  virtual void  Render(CRenderObject* pObj, const SRenderingPassInfo& passInfo) = 0;
  
Class IRenderNode:
   virtual void Render(const struct SRendParams& EntDrawParams, const SRenderingPassInfo& passInfo) = 0;
   
class CRenderMesh : public IRenderMesh:

IndexedMesh:
  can be created directly or loaded from CGF file, before rendering it is converted into IRenderMesh.
  
  virtual CMesh* GetMesh();
  virtual void setMesh();
  
  
class CIndexedMesh : public IIndexedMesh  

CRenderObject:
  Objects using in shader pipeline
  CRenderElement*
  IRenderNode*

Class IRenderElement
    
  
class CRenderElement : public IRenderElement:

    CRenderElement*       m_NextGlobal;
    CRenderElement*       m_PrevGlobal;
    virtual bool  mfPreDraw(SShaderPass* sl) = 0;
    virtual bool  mfDraw(CShader* ef, SShaderPass* sfm);
    
  
class CREMesh : public CRenderElement:
      class CRenderMesh*   m_pRenderMesh;
  
