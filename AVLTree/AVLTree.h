#pragma once

#include <stack>
#include <queue>

#define GetMaxValue(nValue1,nValue2) \
(((nValue1) > (nValue2)) ? (nValue1) : (nValue2))

#define GetHeightOfNode(pNode) \
((pNode == nullptr) ? (-1):(pNode->m_nHeightOfNode))

//�������ڵ�Ķ���
template<typename T>
struct tagNode
{
  T                   m_element;     //���ڵ����洢��Ԫ�ؽڵ�
  struct tagNode<T> * m_pLeftNode;   //ָ�����ӽڵ�
  struct tagNode<T> * m_pRightNode;  //ָ���Һ��ӽڵ�
  struct tagNode<T> * m_pParentNode; //ָ�򸸽ڵ�
  int                 m_nHeightOfNode;//�ڵ�߶�
  tagNode(T value,
          tagNode<T> * pLeftNode = nullptr,
          tagNode<T> *pRightNode = nullptr,
          tagNode<T> *pParentNode = nullptr);
};

template<typename T>
tagNode<T>::tagNode(T value,
                    tagNode<T> * pLeftNode,
                    tagNode<T> *pRightNode,
                    tagNode<T> *pParentNode):
m_element(value),
m_pLeftNode(pLeftNode),
m_pRightNode(pRightNode),
m_pParentNode(pParentNode),
m_nHeightOfNode(0)
{

}


template<typename T>
using BINNODE = tagNode<T>;

template<typename T>
using PBINNODE = tagNode<T>*;


//���ڱ���ʱ���õĺ�������,������ʵ�ֺ������������������,
//�ں������������������ʵ�ֶԽڵ������β���
template<typename T>
class CTraversalFunction
{
public:
  virtual void operator()(PBINNODE<T> pNode) = 0;
};

//����ɾ���ڵ�ʱ�ĺ�������
template<typename T>
class CDeleteNodeFunction
{
public:
    virtual void operator()(PBINNODE<T> pNode) = 0;
};

template<typename T>
class CAVLTree
{
public:
  CAVLTree();
  virtual ~CAVLTree();

  //�������������
  bool PreOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  bool PreOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  //�������������
  bool InOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  bool InOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);

  //�������
  bool PostOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  bool PostOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);

  //�������
  bool LevelOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);

  //�����½ڵ�
  bool InsertNode(T Value);

  //ɾ���ڵ�
  bool DeleteNode(T ValueOfNode,CDeleteNodeFunction<T> & FuncOfDel);

  //���ҽڵ�
  PBINNODE<T> SearchNode(T Element);
  PBINNODE<T> RecursiveSearchNode(T Element);
private:
  //����ƽ�������ʹ��ƽ��
  bool AdjustTree(PBINNODE<T> pInsertNode);

  //�ҵ���
  bool SingleRightRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pLeftNode);

  //����
  bool SingleLeftRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pRightNode);

  //��ȡ�ڵ��������нڵ�ֵ���Ľڵ�
  PBINNODE<T> GetMaxNodeOfLeft(PBINNODE<T> pRootNode);

  //�滻�ڵ�
  bool ReplaceNode(PBINNODE<T> pSrcNode, PBINNODE<T> pMaxNodeOfLeft);
public: 
  PBINNODE<T> m_pRoot;       //ָ����ڵ�,��Ϊ��ʱ��ֵΪnullptr
  unsigned int m_nCountOfNode; //�ڵ����
};

template<typename T>
CAVLTree<T>::CAVLTree<T>():
m_pRoot(nullptr),
m_nCountOfNode(0)
{

}


template<typename T>
CAVLTree<T>::~CAVLTree<T>()
{
}


//************************************************************************
// ��������:  CAVLTree<T>::RecursiveSearchNode
// ��������:  ����ֵΪElement�Ľڵ�
// ����Ȩ��:  public 
// ���ص�ֵ:  ָ����ҵ��Ľڵ�,���û���ҵ���Ϊnullptr
// ��������:  T Element
// ע������:  ���TΪ�Զ����������������Ҫʵ�ֲ�����<��>
//************************************************************************
template<typename T>
PBINNODE<T> CAVLTree<T>::RecursiveSearchNode(T Element)
{
    static PBINNODE<T> pCurrNode = m_pRoot;
    if (pCurrNode->m_element > Element)
    {
        pCurrNode = pCurrNode->m_pLeftNode;
        return RecursiveSearchNode(Element);
    }
    else if (pCurrNode->m_element < Element)
    {
        pCurrNode = pCurrNode->m_pRightNode;
        return RecursiveSearchNode(Element);
    }
    return pCurrNode;
}


//************************************************************************
// ��������:  CAVLTree<T>::ReplaceNode
// ��������:  �滻�ڵ�,��pMaxNodeOfLeftָ��Ľڵ��滻��pDeleteNodeָ��Ľڵ�λ��
// ����Ȩ��:  private 
// ���ص�ֵ:  bool
// ��������:  PBINNODE<T> pDeleteNode:Ҫ��ɾ���Ľڵ�
// ��������:  PBINNODE<T> pMaxNodeOfLeft:�����滻�Ľڵ�
// ע������:  1.���pMaxNodeOfLeftΪ��,��˵����ɾ���ڵ�������,��ʱֱ�ӽ�
//             ��ɾ���ڵ�ĵĸ��ڵ�ʹ�ɾ���ڵ���Һ���������һ�𼴿�
//************************************************************************
template<typename T>
bool CAVLTree<T>::ReplaceNode(PBINNODE<T> pDeleteNode, PBINNODE<T> pMaxNodeOfLeft)
{
    PBINNODE<T> pRootOfDeleteNode = pDeleteNode->m_pParentNode;
    if (pMaxNodeOfLeft == nullptr)
    {
        //��������滻�Ľڵ�Ϊ����˵����ɾ���ڵ�û������,��ֱ��ɾ���ýڵ㼴��
        if (m_pRoot == pDeleteNode)
        {
            //��ɾ���ڵ�Ϊ���ڵ�
            m_pRoot = pDeleteNode->m_pRightNode;
        }
        else
        {
            if (pRootOfDeleteNode->m_pLeftNode == pDeleteNode)
            {
                //��ɾ���ڵ��Ǹ��ڵ�����
                pRootOfDeleteNode->m_pLeftNode = pDeleteNode->m_pRightNode;
            }
            else
            {
                //��ɾ���ڵ��Ǹ��ڵ��Һ���
                pRootOfDeleteNode->m_pRightNode = pDeleteNode->m_pRightNode;
            }

            if (pDeleteNode->m_pRightNode != nullptr)
            {
                pDeleteNode->m_pRightNode->m_pParentNode = pRootOfDeleteNode;
            }
        }
    }
    else if (pDeleteNode->m_pLeftNode == pMaxNodeOfLeft)
    {
        //pMaxNodeOfLeft��pDeleteNode������
        if (pRootOfDeleteNode == nullptr)
        {
            //��ɾ���Ľڵ���ƽ��������ĸ��ڵ�
            m_pRoot = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = nullptr;
            pMaxNodeOfLeft->m_pRightNode = pDeleteNode->m_pRightNode;
            if (pMaxNodeOfLeft->m_pRightNode != nullptr)
            {
                pMaxNodeOfLeft->m_pRightNode->m_pParentNode = pMaxNodeOfLeft;
            }
        }
        else if(pRootOfDeleteNode->m_pLeftNode == pDeleteNode)
        {
            //��ɾ���Ľڵ����丸�ڵ������
            pRootOfDeleteNode->m_pLeftNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
            if (pMaxNodeOfLeft->m_pRightNode != nullptr)
            {
                pMaxNodeOfLeft->m_pRightNode->m_pParentNode = pMaxNodeOfLeft;
            }
        }
        else
        {
            //��ɾ���Ľڵ����丸�ڵ���Һ���
            pRootOfDeleteNode->m_pRightNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
        }
    }
    else
    {
        //pMaxNodeOfLeft��pDeleteNodeû��ֱ�ӹ�ϵ,pMaxNodeOfLeft
        //��û���Һ��ӵ�,����������,����pMaxNodeOfLeft���丸�ڵ���Һ���

        //��ȡ��pMaxNodeOfLeft�ڵ�
        PBINNODE<T> pParnetOfMaxNode = pMaxNodeOfLeft->m_pParentNode;
        pParnetOfMaxNode->m_pRightNode = pMaxNodeOfLeft->m_pLeftNode;
        if (pMaxNodeOfLeft->m_pLeftNode != nullptr)
        {
            pMaxNodeOfLeft->m_pParentNode = pParnetOfMaxNode;
        }

        //��pMaxNodeOfLeft�滻��pDeleteNode�ڵ�
        pMaxNodeOfLeft->m_pLeftNode = pDeleteNode->m_pLeftNode;
        if (pMaxNodeOfLeft->m_pLeftNode != nullptr)
        {
            pMaxNodeOfLeft->m_pLeftNode->m_pParentNode = pMaxNodeOfLeft;
        }

        pMaxNodeOfLeft->m_pRightNode = pDeleteNode->m_pRightNode;
        if (pMaxNodeOfLeft->m_pRightNode != nullptr)
        {
            pMaxNodeOfLeft->m_pRightNode->m_pParentNode = pMaxNodeOfLeft;
        }

        if (pRootOfDeleteNode == nullptr)
        {
            //��ɾ���ڵ�Ϊ���ڵ�
            m_pRoot = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = nullptr;
        }
        else if (pRootOfDeleteNode->m_pLeftNode == pDeleteNode)
        {
            //��ɾ���Ľڵ����丸�ڵ������,�򽫸��ڵ��������ΪpMaxNodeOfLeft
            pRootOfDeleteNode->m_pLeftNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
        }
        else
        {
            //��ɾ���Ľڵ����丸�ڵ���Һ���,�򽫸��ڵ���Һ�����ΪpMaxNodeOfLeft
            pRootOfDeleteNode->m_pRightNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
        }


    }
    return true;
}



//************************************************************************
// ��������:  CAVLTree<T>::GetMaxNodeOfLeft
// ��������:  ��ȡ�ڵ��������нڵ�ֵ���Ľڵ�
// ����Ȩ��:  private 
// ���ص�ֵ:  
// ��������:  PBINNODE<T> pRootNode:�����ҵ��������ڵ�
// ע������: 
//************************************************************************
template<typename T>
PBINNODE<T> CAVLTree<T>::GetMaxNodeOfLeft(PBINNODE<T> pRootNode)
{
    if (pRootNode->m_pLeftNode == nullptr)
    {
        return nullptr;
    }

    PBINNODE<T> pIndex = pRootNode->m_pLeftNode;
    while (pIndex->m_pRightNode != nullptr)
    {
        pIndex = pIndex->m_pRightNode;
    }
    
    return pIndex;
}

//************************************************************************
// ��������: CAVLTree<T>::InOrderTraversal
// ����Ȩ��: public 
// ��������: �������������(�ݹ�ʵ��)
// ����ֵ:   bool:true��ʾ�ɹ�,����ʧ��
// ����:     CTraversalFunction<T> & FuncOfTravs:���ڷ��ʶ������ڵ�ĺ�������
// ����:     PBINNODE<T> pTravsralPos:��ʼ������λ��
// ע��:  
//************************************************************************
template<typename T>
bool CAVLTree<T>::InOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs,
                                            PBINNODE<T> pTravsralPos)
{
    if (pTravsralPos != nullptr)
    {
      InOrderRecursiveTraversal(FuncOfTravs, pTravsralPos->m_pLeftNode);
      FuncOfTravs(pTravsralPos);
      InOrderRecursiveTraversal(FuncOfTravs, pTravsralPos->m_pRightNode);
    }
    return true;
}


//************************************************************************
// ��������: CAVLTree<T>::InOrderTraversal
// ����Ȩ��: public 
// ��������:  �������������(�ǵݹ�ʵ��)
// ����ֵ:   bool
// ����:     CTraversalFunction<T> & FuncOfTravs:���ڷ��ʶ������ڵ�ĺ�������
// ����:     PBINNODE<T> pTravsralPos:��ʼ������λ��
// ע��:     
//************************************************************************
template<typename T>
bool CAVLTree<T>::InOrderTraversal(CTraversalFunction<T> & FuncOfTravs,
                                   PBINNODE<T> pTravsralPos)
{
  std::stack<PBINNODE<T>> tmpStack;
  PBINNODE<T> pCurrNode = pTravsralPos;
  while (true)
  {
      //(1).����ǰ�ڵ��������ջ�����ҽ���ǰ�ڵ��������ջ��ֱ�����Ե�ǰ�ڵ�Ϊ���ڵ�
      //    ������������ߵ�Ҷ�ӽڵ���ջ��Ϊֹ
      while (nullptr != pCurrNode)
      {
          tmpStack.push(pCurrNode);
          pCurrNode = pCurrNode->m_pLeftNode;
      }


      //(2).��ջ���ڵ��ջ���ʣ����ҽ�ջ���ڵ���Һ�����Ϊ��ǰ�ڵ��ظ�(1)����
      //    ���ջ�գ����ʾ�����������������
      if (tmpStack.empty())
      {
          break;
      }

      pCurrNode = tmpStack.top();
      FuncOfTravs(pCurrNode);
      pCurrNode = pCurrNode->m_pRightNode;
      tmpStack.pop();
  }//while
  return true;
}

//************************************************************************
// ��������:  CAVLTree<T>::PreOrderRecuursiveTraversal
// ��������:  �������������(�ݹ�ʵ��)
// ����Ȩ��:  public 
// ���ص�ֵ:  bool
// ��������:  CTraversalFunction<T> & FuncOfTravs:���ʽڵ�ʱʹ�õĺ�������
// ��������:  PBINNODE<T> pTravsralPos:����λ��
// ע������: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::PreOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos)
{
    if (pTravsralPos != nullptr)
    {
        FuncOfTravs(pTravsralPos);
        PreOrderRecursiveTraversal(FuncOfTravs, pTravsralPos->m_pLeftNode);
        PreOrderRecursiveTraversal(FuncOfTravs, pTravsralPos->m_pRightNode);
    }
    return true;
}



//************************************************************************
// ��������:  PreOrderTraversal
// ��������:  �������������(�ǵݹ�ʵ��)
// ����Ȩ��:  public 
// ���ص�ֵ:  bool
// ��������:  CTraversalFunction<T> & FuncOfTravs:���ʽڵ�ʱʹ�õĺ�������
// ��������:  PBINNODE<T> pTravsralPos:����λ��
// ע������: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::PreOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos)
{
    std::stack<PBINNODE<T>> tmpStack;
    PBINNODE<T> pCurrNode = pTravsralPos;
    while (pCurrNode != nullptr)
    {
        FuncOfTravs(pCurrNode); 
        if (pCurrNode->m_pRightNode != nullptr)
        {
            //��ǰ�Ѿ����ʽڵ���Һ��Ӵ�������ջ
            tmpStack.push(pCurrNode->m_pRightNode);
        }
        
        if (pCurrNode->m_pLeftNode == nullptr)
        {
            if (tmpStack.empty())
            {
                pCurrNode = nullptr;
            }
            else
            {   
                pCurrNode = tmpStack.top();
                tmpStack.pop();
            }
        }
        else
        {
            pCurrNode = pCurrNode->m_pLeftNode;
        }
    }
    return true;
}


//************************************************************************
// ��������:  CAVLTree<T>::PostOrderRecursiveTraversal
// ��������:  ��������������(�ݹ�ʵ��)
// ����Ȩ��:  public 
// ���ص�ֵ:  bool
// ��������:  CTraversalFunction<T> & FuncOfTravs:���ʽڵ�ʱʹ�õĺ�������
// ��������:  PBINNODE<T> pTravsralPos:����λ��
// ע������: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::PostOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos)
{
    if (pTravsralPos)
    {
        PostOrderRecursiveTraversal(FuncOfTravs, pTravsralPos->m_pLeftNode);
        PostOrderRecursiveTraversal(FuncOfTravs, pTravsralPos->m_pRightNode);
        FuncOfTravs(pTravsralPos);
    }
    return true;
}

//************************************************************************
// ��������:  CAVLTree<T>::PostOrderTraversal
// ��������:  ��������������(�ǵݹ�ʵ��)
// ����Ȩ��:  public 
// ���ص�ֵ:  bool
// ��������:  CTraversalFunction<T> & FuncOfTravs:���ʽڵ�ʱʹ�õĺ�������
// ��������:  PBINNODE<T> pTravsralPos:����λ��
// ע������: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::PostOrderTraversal(CTraversalFunction<T> & FuncOfTravs, 
                                     PBINNODE<T> pTravsralPos)
{
    std::stack<PBINNODE<T>> tmpStack;
    PBINNODE<T> pCurrNode = pTravsralPos;
    PBINNODE<T> pLastVisit = nullptr; //��¼�ϴη��ʵĽڵ�
    while (true)
    {
        //(1).����ǰ�ڵ��������ջ�����ҽ���ǰ�ڵ��������ջ��ֱ�����Ե�ǰ�ڵ�Ϊ���ڵ�
        //    ������������ߵ�Ҷ�ӽڵ���ջ��Ϊֹ
        while (nullptr != pCurrNode)
        {
            tmpStack.push(pCurrNode);
            pCurrNode = pCurrNode->m_pLeftNode;
        }

        if (tmpStack.empty())
        {
            //ջ���޽ڵ����ʾ�����������������
            break;
        }

        pCurrNode = tmpStack.top();
        if (nullptr != pCurrNode->m_pRightNode &&
            pCurrNode->m_pRightNode != pLastVisit)
        {
            //(2)��ǰ�ڵ�����������ڲ���û�б����ʣ�������ظ�(1)������
            pCurrNode = pCurrNode->m_pRightNode;
        }
        else
        {
            //(3)�����������ڻ����������Ѿ������ʣ�����ʵ�ǰ�ڵ�
            FuncOfTravs(pCurrNode);
            pLastVisit = pCurrNode;
            tmpStack.pop();
            pCurrNode = nullptr;
        }
    }
    return true;
}



//************************************************************************
// ��������:  LevelOrderTraversal
// ��������:  �������
// ����Ȩ��:  public 
// ���ص�ֵ:  bool
// ��������:  CTraversalFunction<T> & FuncOfTravs:���ʽڵ�ʱʹ�õĺ�������
// ��������:  PBINNODE<T> pTravsralPos:����λ��
// ע������:  1.ÿ��������ұ���
//************************************************************************
template<typename T>
bool CAVLTree<T>::LevelOrderTraversal(CTraversalFunction<T> & FuncOfTravs, 
                                      PBINNODE<T> pTravsralPos)
{
    if (pTravsralPos == nullptr)
    {
        return false;
    }

    PBINNODE<T> pCurrNode = pTravsralPos;
    std::deque<PBINNODE<T>> tmpQueue;

    tmpQueue.push_front(pCurrNode);

    while (!tmpQueue.empty())
    {
        pCurrNode = tmpQueue.front();
        tmpQueue.pop_front();
        FuncOfTravs(pCurrNode);

        //������������ÿ��������ұ���,�����Ѿ����ʹ��ڵ������Ҫ�������
        if (pCurrNode->m_pLeftNode != nullptr)
        {
            tmpQueue.push_back(pCurrNode->m_pLeftNode);
        }

        if (pCurrNode->m_pRightNode != nullptr)
        {
            tmpQueue.push_back(pCurrNode->m_pRightNode);
        }

    }
    return true;
}



//************************************************************************
// ��������: CAVLTree<T>::InsertNode
// ����Ȩ��: public 
// ��������: ��ƽ��������в���һ���½ڵ�
// ����ֵ:   bool:����Ѿ�������ͬ�Ľڵ�ֵ�򷵻�false
// ����:     T value:Ҫ����Ľڵ�Ԫ��
// ע��:     1.��������Ѿ�������ͬ�Ľڵ��򲻲���
//          2.�Զ����Ԫ��T����Ҫ����>,<�����������
//          3.Ҷ�ڵ�ĸ߶���Ϊ0,��Ϊ���½ڵ㹹��ʱ�Ѿ����ڵ�߶���Ϊ0,
//            ��������������²���ڵ�ĸ߶Ȳ��ڽ��и�0ֵ
//************************************************************************
template<typename T>
bool CAVLTree<T>::InsertNode(T Value)
{
  PBINNODE<T> pNewNode = new BINNODE<T>(Value);
  if (pNewNode == nullptr)
  {
    return false;
  }

  if (m_pRoot == nullptr)
  {
    m_pRoot = pNewNode;
    m_nCountOfNode++;
    return true;
  }

  //��ʼ����������,Ѱ�Һ��ʵĲ���λ��
  PBINNODE<T> pIndexNode = m_pRoot;
  while (pIndexNode != nullptr)
  {
    if (pIndexNode->m_element > pNewNode->m_element)
    {
      //Ҫ����Ľڵ�ֵС�ڵ�ǰ�ڵ�����ǰ�ڵ��������Ѱ��,
      //�����ǰ�ڵ�����Ӳ�������ֱ�Ӳ����½ڵ�
      if (pIndexNode->m_pLeftNode == nullptr)
      {
        pIndexNode->m_pLeftNode = pNewNode;
        pNewNode->m_pParentNode = pIndexNode;
        break;
      }
      pIndexNode = pIndexNode->m_pLeftNode;
    }
    else if (pIndexNode->m_element < pNewNode->m_element)
    {
      //Ҫ����Ľڵ�ֵ���ڵ�ǰ�ڵ�����ǰ�ڵ��������Ѱ��
      if (pIndexNode->m_pRightNode == nullptr)
      {
        pIndexNode->m_pRightNode = pNewNode;
        pNewNode->m_pParentNode = pIndexNode;
        break;
      }
      pIndexNode = pIndexNode->m_pRightNode;
    }
    else
    {
      //Ҫ����Ľڵ�ֵ�͵�ǰ�ڵ�ֵ�����˵���ڵ��Ѿ�����
      return false;
    }
  }

  m_nCountOfNode++;
  //�����½ڵ��,���½ڵ�ĸ��ڵ㵽���ڵ�Ľڵ�߶ȶ���Ҫ����
  //�����²���ڵ����ܻᵼ��ƽ�������ʧȥƽ��,��������Ҫ����
  //����,ʹ�䴦��ƽ��״̬
  return AdjustTree(pNewNode);
}

//************************************************************************
// ��������:  SingleRightRotate
// ��������:  �ҵ���
// ����Ȩ��:  public 
// ���ص�ֵ:  bool
// ��������:  PBINNODE<T> pUnbalanceNode:ʧȥƽ��Ľڵ�
// ��������:  PBINNODE<T> pLeftNode:ʧ��ڵ������
// ע������: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::SingleRightRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pLeftNode)
{
    //�ж�pLeftNode�Ƿ���pUnbalanceNode������,��ֹ������
    if (pUnbalanceNode->m_pLeftNode != pLeftNode)
    {
        return false;
    }

    //���ҵ�����:pUnbalanceNodeָ���ʧ��ڵ㾭�������󽫱�ΪpLeftNode���Һ���,���pleftNode��
    //���Һ��Ӵ����򽫱�ΪpUnbalanceNode������
    PBINNODE<T> pParentOfUnbalance = pUnbalanceNode->m_pParentNode;

    if (pParentOfUnbalance != nullptr)
    {
        //ʧ��ڵ㲻�Ƕ������ĸ��ڵ�
        if (pParentOfUnbalance->m_pLeftNode == pUnbalanceNode)
        {
            //ʧ��ڵ��и��ڵ�,�������丸�ڵ������
            pParentOfUnbalance->m_pLeftNode = pLeftNode;
        }
        else
        {
            //ʧ��ڵ��и��ڵ�,�������丸�ڵ���Һ���
            pParentOfUnbalance->m_pRightNode = pLeftNode;
        }
        //����ʧ��ڵ�����ӵĸ��ڵ�Ϊʧ��ڵ�ĸ��ڵ�
        pLeftNode->m_pParentNode = pParentOfUnbalance;
    }
    else
    {
        //ʧ��ڵ�Ϊ�������ĸ��ڵ�
        pLeftNode->m_pParentNode = nullptr;
        m_pRoot = pLeftNode;
    }

    //����ʧ��ڵ�ĸ��ڵ�Ϊ��������
    pUnbalanceNode->m_pParentNode = pLeftNode;

    //����ʧ��ڵ������Ϊ����ǰ����(pLeftNode)���Һ���
    pUnbalanceNode->m_pLeftNode = pLeftNode->m_pRightNode;

    //���ʧ��ڵ����ӵ��Һ��Ӵ���,���丸�ڵ�����Ϊʧ��ڵ�
    if (pLeftNode->m_pRightNode != nullptr)
    {
        pLeftNode->m_pRightNode->m_pParentNode = pUnbalanceNode;
    }

    //����ʧ��ڵ��Һ��ӵ�Ϊʧ��ڵ�
    pLeftNode->m_pRightNode = pUnbalanceNode;
    
    //���¼���ṹ������pLeftNode��pUnbalanceNode�Ľڵ�߶�
    //����Ҫע�⾭���ҵ�����ֻ��pUnbalanceNode��pLeftNode�Ľڵ�߶�
    //�����˱仯,ʧ���������´ﵽƽ��,����ʧ�������ĸ��ڵ�Ľڵ�߶�����
    //���ֲ���,����pUnbalanceNodeָ��Ľڵ�Ľڵ�߶�ֵ������е���

    //����������pLeftNodeָ��Ľڵ��Ϊ�µ�ƽ�������ĸ��ڵ�,pUnbalanceNode
    //ָ��Ľڵ��ΪpLeftNode���Һ��ӽڵ�,��������Ҫ�ȼ���pUnbalanceNode��
    //�ڵ�߶�ֵ,Ȼ���ڼ���pLeftNode�Ľڵ�߶�ֵ
    int nHeightOfLeft = GetHeightOfNode(pUnbalanceNode->m_pLeftNode);
    int nHeightOfRight = GetHeightOfNode(pUnbalanceNode->m_pRightNode);
    pUnbalanceNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

    nHeightOfLeft = GetHeightOfNode(pLeftNode->m_pLeftNode);
    nHeightOfRight = GetHeightOfNode(pLeftNode->m_pRightNode);
    pLeftNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft,nHeightOfRight)+1;

    //ע��:�ҽ�Ҷ�ڵ�߶���Ϊ0,�����������߶�ʱҪ��1
    return true;
}


//************************************************************************
// ��������:  CAVLTree<T>::SingleLeftRotate
// ��������:  ����
// ����Ȩ��:  private 
// ���ص�ֵ:  bool
// ��������:  PBINNODE<T> pUnbalanceNode:ʧ��ڵ�
// ��������:  PBINNODE<T> pRightNode:ʧ��ڵ���Һ���
// ע������: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::SingleLeftRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pRightNode)
{
    //�ж�pLeftNode�Ƿ���pUnbalanceNode���Һ���,��ֹ������
    if (pUnbalanceNode->m_pRightNode != pRightNode)
    {
        return false;
    }

    PBINNODE<T> pParentOfUnbalance = pUnbalanceNode->m_pParentNode;

    if (pParentOfUnbalance != nullptr)
    {
        //ʧ��ڵ㲻�Ƕ������ĸ��ڵ�
        if (pParentOfUnbalance->m_pLeftNode == pUnbalanceNode)
        {
            //ʧ��ڵ����丸�ڵ������
            pParentOfUnbalance->m_pLeftNode = pRightNode;
        }
        else
        {
            //ʧ��ڵ����丸�ڵ���Һ���
            pParentOfUnbalance->m_pRightNode = pRightNode;
        }

        pRightNode->m_pParentNode = pParentOfUnbalance;
    }
    else
    {
        //ʧ��ڵ��Ƕ������ĸ��ڵ�
        pRightNode->m_pParentNode = nullptr;
        m_pRoot = pRightNode;
    }

    //����ʧ��ڵ�Ϊ���Һ��ӵ�����,���ҽ�ʧ��ڵ��Һ�����ǰ������
    //��Ϊʧ��ڵ���Һ���
    pUnbalanceNode->m_pRightNode = pRightNode->m_pLeftNode;
    //ʧ��ڵ��Һ�����ǰ����������������丸�ڵ���Ϊʧ��ڵ�
    if (pRightNode->m_pLeftNode != nullptr)
    {
        pRightNode->m_pLeftNode->m_pParentNode = pUnbalanceNode;
    }
    
    //����ʧ��ڵ�ĸ��ڵ�Ϊ����ǰ���Һ���
    pUnbalanceNode->m_pParentNode = pRightNode;
    pRightNode->m_pLeftNode = pUnbalanceNode;

    //��ʱ�������Ѿ�ƽ��,�������pUnbalanceNode��pRightNode�Ľڵ�߶�
    //ע��:pUnbalanceNodeָ��Ľڵ㾭������������ʧ�������ĸ��ڵ�,pRightNode
    //    ָ��Ľڵ㾭���������Ϊ�µ�ƽ�������ĸ��ڵ�,������������ȵ���pUnbalanceNode
    //    ָ��Ľڵ�߶�,Ȼ���ڵ���pRightNode��ָ��Ľڵ�߶�
    int nHeightOfLeft = GetHeightOfNode(pUnbalanceNode->m_pLeftNode);
    int nHeightOfRight = GetHeightOfNode(pUnbalanceNode->m_pRightNode);
    pUnbalanceNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

    nHeightOfRight = GetHeightOfNode(pRightNode->m_pLeftNode);
    nHeightOfLeft = GetHeightOfNode(pRightNode->m_pRightNode);
    pRightNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

    return true;
}

//************************************************************************
// ��������:  CAVLTree<T>::AdjustTree
// ��������:  ����������ʹ��ƽ��
// ����Ȩ��:  private 
// ���ص�ֵ:  bool
// ��������:  PBINNODE<T> pInsertNode:�������Ľڵ�
// ע������:   
//************************************************************************
template<typename T>
bool CAVLTree<T>::AdjustTree(PBINNODE<T> pInsertNode)
{
    if (pInsertNode == nullptr)
    {
        return false;
    }

    PBINNODE<T> pAdjustPos = pInsertNode;
    PBINNODE<T> pSubNode = nullptr;
    //��pInsertNode�ڵ㿪ʼ,һֱ���������ڵ����
    while (pAdjustPos != nullptr)
    {
        //�����½ڵ��ᵼ�������ڵ�ĸ߶ȷ����仯,����Ҫ�����ڵ�߶�
        int nHeightOfLeft = GetHeightOfNode(pAdjustPos->m_pLeftNode);
        int nHeightOfRight = GetHeightOfNode(pAdjustPos->m_pRightNode);
        
        //Ҷ�ڵ�ĸ߶���Ϊ0,�����������ڵ�߶�ʱҪ��1
        pAdjustPos->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

        //���ݵ�ǰ�ڵ�����������߶Ȳ����жϲ���������ת��ʽ������
        int nDiffValue = nHeightOfLeft - nHeightOfRight;
        if (nDiffValue < -1)
        {
            //��ʱ��ǰ�ڵ���������ϲ�����½ڵ㵼�¶�������ƽ��
            pSubNode = pAdjustPos->m_pRightNode;
            if (GetHeightOfNode(pSubNode->m_pLeftNode) > GetHeightOfNode(pSubNode->m_pRightNode))
            {
                //�ҽڵ�����������ҽڵ����������,��������Ҫ��������˫��
                SingleRightRotate(pSubNode, pSubNode->m_pLeftNode);
                SingleLeftRotate(pAdjustPos, pAdjustPos->m_pRightNode);
            }
            else
            {
                //�ҽڵ�����������ҽڵ����������,��������Ҫ��������
                SingleLeftRotate(pAdjustPos, pAdjustPos->m_pRightNode);
            }
        }
        else if (nDiffValue > 1)
        {
            pSubNode = pAdjustPos->m_pLeftNode;
            if (GetHeightOfNode(pSubNode->m_pLeftNode) > GetHeightOfNode(pSubNode->m_pRightNode))
            {
                //��ڵ������������ڵ����������,��������Ҫ���ҵ���
                SingleRightRotate(pAdjustPos, pAdjustPos->m_pLeftNode);
            }
            else
            {
                //��ڵ������������ڵ����������,��������Ҫ��������˫��
                SingleLeftRotate(pSubNode, pSubNode->m_pRightNode);
                SingleRightRotate(pAdjustPos, pAdjustPos->m_pLeftNode);
            }
        }
        else
        {
            //��ǰ�ڵ�û��ʧ���������,���ж��丸�ڵ��Ƿ���Ҫ����
            pAdjustPos = pAdjustPos->m_pParentNode;
        }
    }
    return true;
}


//************************************************************************
// ��������:  CAVLTree<T>::DeleteNode
// ��������:  �Ƴ��ڵ�
// ����Ȩ��:  public 
// ���ص�ֵ:  bool:�ɹ�����true,���򷵻�false
// ��������:  T ValueOfNode:Ҫɾ���Ľڵ�ֵ
// ��������:  CDeleteNodeFunction<T> & FuncOfDel:�����ͷŽڵ��ڴ�ĺ�������
//************************************************************************
template<typename T>
bool CAVLTree<T>::DeleteNode(T ValueOfNode, CDeleteNodeFunction<T> & FuncOfDel)
{
    //��Ѱ�Ҷ�Ӧ�ڵ�
    PBINNODE<T> pDeleteNode = SearchNode(ValueOfNode);
    if (pDeleteNode == nullptr)
    {
        return false;
    }

    //�ҵ���Ӧ�ڵ�,����ɾ��
    PBINNODE<T> pMaxNodeOfLeft = GetMaxNodeOfLeft(pDeleteNode);
    PBINNODE<T> pAdjustPos = nullptr;
    if (pMaxNodeOfLeft != nullptr)
    {
        pAdjustPos = pMaxNodeOfLeft;
    }
    else
    {
        pAdjustPos = pDeleteNode->m_pParentNode;
    }

    bool bRet = ReplaceNode(pDeleteNode, pMaxNodeOfLeft);
    if (!bRet)
    {
        return bRet;
    }

    //�ͷŽڵ��ڴ�
    FuncOfDel(pDeleteNode);

    //����������ʹ��ƽ��
    AdjustTree(pAdjustPos);
    return bRet;
}



//************************************************************************
// ��������:  CAVLTree<T>::SearchNode
// ��������:  ���ҽڵ�
// ����Ȩ��:  public 
// ���ص�ֵ:  PBINNODE<T>:û���ҵ��򷵻�nullptr,���򷵻ض�Ӧ�ڵ��ָ��
// ��������:  T Element:�ڵ�Ԫ��ֵ
// ע������:  1.�Զ������������T����Ҫ����>,<�����������
//************************************************************************
template<typename T>
PBINNODE<T> CAVLTree<T>::SearchNode(T Element)
{
    PBINNODE<T> pIndex = m_pRoot;
    while (pIndex != nullptr)
    {
        if (pIndex->m_element > Element)
        {
            //��ǰ�ڵ��Ԫ��ֵ��Element��,����ǰ�ڵ������������
            pIndex = pIndex->m_pLeftNode;
            continue;
        }

        if (pIndex->m_element < Element)
        {
            //��ǰ�ڵ��Ԫ��ֵ��Element��,����ǰ�ڵ������������
            pIndex = pIndex->m_pRightNode;
            continue;
        }

        break;
    }

    return pIndex;
}
