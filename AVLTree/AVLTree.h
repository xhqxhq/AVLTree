#pragma once

#include <stack>
#include <queue>

#define GetMaxValue(nValue1,nValue2) \
(((nValue1) > (nValue2)) ? (nValue1) : (nValue2))

#define GetHeightOfNode(pNode) \
((pNode == nullptr) ? (-1):(pNode->m_nHeightOfNode))

//二叉树节点的定义
template<typename T>
struct tagNode
{
  T                   m_element;     //树节点所存储的元素节点
  struct tagNode<T> * m_pLeftNode;   //指向左孩子节点
  struct tagNode<T> * m_pRightNode;  //指向右孩子节点
  struct tagNode<T> * m_pParentNode; //指向父节点
  int                 m_nHeightOfNode;//节点高度
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


//用于遍历时所用的函数对象,由子类实现函数调用运算符的重载,
//在函数调用运算符中子类实现对节点进行如何操作
template<typename T>
class CTraversalFunction
{
public:
  virtual void operator()(PBINNODE<T> pNode) = 0;
};

//用于删除节点时的函数对象
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

  //先序遍历二叉树
  bool PreOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  bool PreOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  //中序遍历二叉树
  bool InOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  bool InOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);

  //后序遍历
  bool PostOrderRecursiveTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);
  bool PostOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);

  //层序遍历
  bool LevelOrderTraversal(CTraversalFunction<T> & FuncOfTravs, PBINNODE<T> pTravsralPos);

  //插入新节点
  bool InsertNode(T Value);

  //删除节点
  bool DeleteNode(T ValueOfNode,CDeleteNodeFunction<T> & FuncOfDel);

  //查找节点
  PBINNODE<T> SearchNode(T Element);
  PBINNODE<T> RecursiveSearchNode(T Element);
private:
  //调整平衡二叉树使其平衡
  bool AdjustTree(PBINNODE<T> pInsertNode);

  //右单旋
  bool SingleRightRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pLeftNode);

  //左单旋
  bool SingleLeftRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pRightNode);

  //获取节点左子树中节点值最大的节点
  PBINNODE<T> GetMaxNodeOfLeft(PBINNODE<T> pRootNode);

  //替换节点
  bool ReplaceNode(PBINNODE<T> pSrcNode, PBINNODE<T> pMaxNodeOfLeft);
public: 
  PBINNODE<T> m_pRoot;       //指向根节点,树为空时此值为nullptr
  unsigned int m_nCountOfNode; //节点个数
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
// 函数名称:  CAVLTree<T>::RecursiveSearchNode
// 函数功能:  查找值为Element的节点
// 所属权限:  public 
// 返回的值:  指向查找到的节点,如果没有找到则为nullptr
// 函数参数:  T Element
// 注意事项:  如果T为自定义数据类型则必须要实现操作符<和>
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
// 函数名称:  CAVLTree<T>::ReplaceNode
// 函数功能:  替换节点,把pMaxNodeOfLeft指向的节点替换到pDeleteNode指向的节点位置
// 所属权限:  private 
// 返回的值:  bool
// 函数参数:  PBINNODE<T> pDeleteNode:要被删除的节点
// 函数参数:  PBINNODE<T> pMaxNodeOfLeft:用于替换的节点
// 注意事项:  1.如果pMaxNodeOfLeft为空,则说明待删除节点无左孩子,此时直接将
//             待删除节点的的父节点和待删除节点的右孩子链接在一起即可
//************************************************************************
template<typename T>
bool CAVLTree<T>::ReplaceNode(PBINNODE<T> pDeleteNode, PBINNODE<T> pMaxNodeOfLeft)
{
    PBINNODE<T> pRootOfDeleteNode = pDeleteNode->m_pParentNode;
    if (pMaxNodeOfLeft == nullptr)
    {
        //如果用来替换的节点为空则说明待删除节点没有左孩子,则直接删除该节点即可
        if (m_pRoot == pDeleteNode)
        {
            //待删除节点为根节点
            m_pRoot = pDeleteNode->m_pRightNode;
        }
        else
        {
            if (pRootOfDeleteNode->m_pLeftNode == pDeleteNode)
            {
                //待删除节点是父节的左孩子
                pRootOfDeleteNode->m_pLeftNode = pDeleteNode->m_pRightNode;
            }
            else
            {
                //待删除节点是父节的右孩子
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
        //pMaxNodeOfLeft是pDeleteNode的左孩子
        if (pRootOfDeleteNode == nullptr)
        {
            //待删除的节点是平衡二叉树的根节点
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
            //待删除的节点是其父节点的左孩子
            pRootOfDeleteNode->m_pLeftNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
            if (pMaxNodeOfLeft->m_pRightNode != nullptr)
            {
                pMaxNodeOfLeft->m_pRightNode->m_pParentNode = pMaxNodeOfLeft;
            }
        }
        else
        {
            //待删除的节点是其父节点的右孩子
            pRootOfDeleteNode->m_pRightNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
        }
    }
    else
    {
        //pMaxNodeOfLeft和pDeleteNode没有直接关系,pMaxNodeOfLeft
        //是没有右孩子的,可能有左孩子,并且pMaxNodeOfLeft是其父节点的右孩子

        //先取下pMaxNodeOfLeft节点
        PBINNODE<T> pParnetOfMaxNode = pMaxNodeOfLeft->m_pParentNode;
        pParnetOfMaxNode->m_pRightNode = pMaxNodeOfLeft->m_pLeftNode;
        if (pMaxNodeOfLeft->m_pLeftNode != nullptr)
        {
            pMaxNodeOfLeft->m_pParentNode = pParnetOfMaxNode;
        }

        //用pMaxNodeOfLeft替换掉pDeleteNode节点
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
            //待删除节点为根节点
            m_pRoot = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = nullptr;
        }
        else if (pRootOfDeleteNode->m_pLeftNode == pDeleteNode)
        {
            //待删除的节点是其父节点的左孩子,则将父节点的左孩子置为pMaxNodeOfLeft
            pRootOfDeleteNode->m_pLeftNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
        }
        else
        {
            //待删除的节点是其父节点的右孩子,则将父节点的右孩子置为pMaxNodeOfLeft
            pRootOfDeleteNode->m_pRightNode = pMaxNodeOfLeft;
            pMaxNodeOfLeft->m_pParentNode = pRootOfDeleteNode;
        }


    }
    return true;
}



//************************************************************************
// 函数名称:  CAVLTree<T>::GetMaxNodeOfLeft
// 函数功能:  获取节点左子树中节点值最大的节点
// 所属权限:  private 
// 返回的值:  
// 函数参数:  PBINNODE<T> pRootNode:待查找的子树根节点
// 注意事项: 
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
// 函数名称: CAVLTree<T>::InOrderTraversal
// 访问权限: public 
// 函数功能: 中序遍历二叉树(递归实现)
// 返回值:   bool:true表示成功,否则失败
// 参数:     CTraversalFunction<T> & FuncOfTravs:用于访问二叉树节点的函数对象
// 参数:     PBINNODE<T> pTravsralPos:开始遍历的位置
// 注意:  
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
// 函数名称: CAVLTree<T>::InOrderTraversal
// 访问权限: public 
// 函数功能:  中序遍历二叉树(非递归实现)
// 返回值:   bool
// 参数:     CTraversalFunction<T> & FuncOfTravs:用于访问二叉树节点的函数对象
// 参数:     PBINNODE<T> pTravsralPos:开始遍历的位置
// 注意:     
//************************************************************************
template<typename T>
bool CAVLTree<T>::InOrderTraversal(CTraversalFunction<T> & FuncOfTravs,
                                   PBINNODE<T> pTravsralPos)
{
  std::stack<PBINNODE<T>> tmpStack;
  PBINNODE<T> pCurrNode = pTravsralPos;
  while (true)
  {
      //(1).将当前节点的左孩子入栈，并且将当前节点的左孩子入栈，直到将以当前节点为根节点
      //    二叉树的最左边的叶子节点入栈后为止
      while (nullptr != pCurrNode)
      {
          tmpStack.push(pCurrNode);
          pCurrNode = pCurrNode->m_pLeftNode;
      }


      //(2).将栈顶节点出栈访问，并且将栈顶节点的右孩子作为当前节点重复(1)步骤
      //    如果栈空，则表示整个二叉树遍历完毕
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
// 函数名称:  CAVLTree<T>::PreOrderRecuursiveTraversal
// 函数功能:  先序遍历二叉树(递归实现)
// 所属权限:  public 
// 返回的值:  bool
// 函数参数:  CTraversalFunction<T> & FuncOfTravs:访问节点时使用的函数对象
// 函数参数:  PBINNODE<T> pTravsralPos:遍历位置
// 注意事项: 
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
// 函数名称:  PreOrderTraversal
// 函数功能:  先序遍历二叉树(非递归实现)
// 所属权限:  public 
// 返回的值:  bool
// 函数参数:  CTraversalFunction<T> & FuncOfTravs:访问节点时使用的函数对象
// 函数参数:  PBINNODE<T> pTravsralPos:遍历位置
// 注意事项: 
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
            //当前已经访问节点的右孩子存在则入栈
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
// 函数名称:  CAVLTree<T>::PostOrderRecursiveTraversal
// 函数功能:  后续遍历二叉树(递归实现)
// 所属权限:  public 
// 返回的值:  bool
// 函数参数:  CTraversalFunction<T> & FuncOfTravs:访问节点时使用的函数对象
// 函数参数:  PBINNODE<T> pTravsralPos:遍历位置
// 注意事项: 
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
// 函数名称:  CAVLTree<T>::PostOrderTraversal
// 函数功能:  后续遍历二叉树(非递归实现)
// 所属权限:  public 
// 返回的值:  bool
// 函数参数:  CTraversalFunction<T> & FuncOfTravs:访问节点时使用的函数对象
// 函数参数:  PBINNODE<T> pTravsralPos:遍历位置
// 注意事项: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::PostOrderTraversal(CTraversalFunction<T> & FuncOfTravs, 
                                     PBINNODE<T> pTravsralPos)
{
    std::stack<PBINNODE<T>> tmpStack;
    PBINNODE<T> pCurrNode = pTravsralPos;
    PBINNODE<T> pLastVisit = nullptr; //记录上次访问的节点
    while (true)
    {
        //(1).将当前节点的左孩子入栈，并且将当前节点的左孩子入栈，直到将以当前节点为根节点
        //    二叉树的最左边的叶子节点入栈后为止
        while (nullptr != pCurrNode)
        {
            tmpStack.push(pCurrNode);
            pCurrNode = pCurrNode->m_pLeftNode;
        }

        if (tmpStack.empty())
        {
            //栈中无节点则表示整个二叉树遍历完成
            break;
        }

        pCurrNode = tmpStack.top();
        if (nullptr != pCurrNode->m_pRightNode &&
            pCurrNode->m_pRightNode != pLastVisit)
        {
            //(2)当前节点的右子树存在并且没有被访问，则继续重复(1)处操作
            pCurrNode = pCurrNode->m_pRightNode;
        }
        else
        {
            //(3)右子树不存在或者右子树已经被访问，则访问当前节点
            FuncOfTravs(pCurrNode);
            pLastVisit = pCurrNode;
            tmpStack.pop();
            pCurrNode = nullptr;
        }
    }
    return true;
}



//************************************************************************
// 函数名称:  LevelOrderTraversal
// 函数功能:  层序遍历
// 所属权限:  public 
// 返回的值:  bool
// 函数参数:  CTraversalFunction<T> & FuncOfTravs:访问节点时使用的函数对象
// 函数参数:  PBINNODE<T> pTravsralPos:遍历位置
// 注意事项:  1.每层从左向右遍历
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

        //这里层序遍历的每层从左向右遍历,所以已经访问过节点的左孩子要先入队列
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
// 函数名称: CAVLTree<T>::InsertNode
// 访问权限: public 
// 函数功能: 向平衡二叉树中插入一个新节点
// 返回值:   bool:如果已经存在相同的节点值则返回false
// 参数:     T value:要插入的节点元素
// 注意:     1.如果树中已经存在相同的节点则不插入
//          2.自定义的元素T必须要重载>,<这两个运算符
//          3.叶节点的高度视为0,因为在新节点构造时已经将节点高度置为0,
//            所以在这里对于新插入节点的高度不在进行赋0值
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

  //开始遍历二叉树,寻找合适的插入位置
  PBINNODE<T> pIndexNode = m_pRoot;
  while (pIndexNode != nullptr)
  {
    if (pIndexNode->m_element > pNewNode->m_element)
    {
      //要插入的节点值小于当前节点则向当前节点的左子树寻找,
      //如果当前节点的左孩子不存在则直接插入新节点
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
      //要插入的节点值大于当前节点则向当前节点的右子树寻找
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
      //要插入的节点值和当前节点值相等则说明节点已经存在
      return false;
    }
  }

  m_nCountOfNode++;
  //插入新节点后,从新节点的父节点到根节点的节点高度都需要调整
  //并且新插入节点后可能会导致平衡二叉树失去平衡,所以这里要进行
  //调整,使其处于平衡状态
  return AdjustTree(pNewNode);
}

//************************************************************************
// 函数名称:  SingleRightRotate
// 函数功能:  右单旋
// 所属权限:  public 
// 返回的值:  bool
// 函数参数:  PBINNODE<T> pUnbalanceNode:失去平衡的节点
// 函数参数:  PBINNODE<T> pLeftNode:失衡节点的左孩子
// 注意事项: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::SingleRightRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pLeftNode)
{
    //判断pLeftNode是否是pUnbalanceNode的左孩子,防止被忽悠
    if (pUnbalanceNode->m_pLeftNode != pLeftNode)
    {
        return false;
    }

    //在右单旋中:pUnbalanceNode指向的失衡节点经过调整后将变为pLeftNode的右孩子,如果pleftNode的
    //的右孩子存在则将变为pUnbalanceNode的左孩子
    PBINNODE<T> pParentOfUnbalance = pUnbalanceNode->m_pParentNode;

    if (pParentOfUnbalance != nullptr)
    {
        //失衡节点不是二叉树的根节点
        if (pParentOfUnbalance->m_pLeftNode == pUnbalanceNode)
        {
            //失衡节点有父节点,并且是其父节点的左孩子
            pParentOfUnbalance->m_pLeftNode = pLeftNode;
        }
        else
        {
            //失衡节点有父节点,并且是其父节点的右孩子
            pParentOfUnbalance->m_pRightNode = pLeftNode;
        }
        //调整失衡节点的左孩子的父节点为失衡节点的父节点
        pLeftNode->m_pParentNode = pParentOfUnbalance;
    }
    else
    {
        //失衡节点为二叉树的根节点
        pLeftNode->m_pParentNode = nullptr;
        m_pRoot = pLeftNode;
    }

    //重置失衡节点的父节点为它的左孩子
    pUnbalanceNode->m_pParentNode = pLeftNode;

    //重置失衡节点的左孩子为其先前左孩子(pLeftNode)的右孩子
    pUnbalanceNode->m_pLeftNode = pLeftNode->m_pRightNode;

    //如果失衡节点左孩子的右孩子存在,则将其父节点设置为失衡节点
    if (pLeftNode->m_pRightNode != nullptr)
    {
        pLeftNode->m_pRightNode->m_pParentNode = pUnbalanceNode;
    }

    //重置失衡节点右孩子的为失衡节点
    pLeftNode->m_pRightNode = pUnbalanceNode;
    
    //重新计算结构调整后pLeftNode和pUnbalanceNode的节点高度
    //这里要注意经过右单旋后只有pUnbalanceNode和pLeftNode的节点高度
    //发生了变化,失衡子树重新达到平衡,所以失衡子树的父节点的节点高度依旧
    //保持不变,所以pUnbalanceNode指向的节点的节点高度值无需进行调整

    //经过调整后pLeftNode指向的节点成为新的平衡子树的根节点,pUnbalanceNode
    //指向的节点成为pLeftNode的右孩子节点,所以这里要先计算pUnbalanceNode的
    //节点高度值,然后在计算pLeftNode的节点高度值
    int nHeightOfLeft = GetHeightOfNode(pUnbalanceNode->m_pLeftNode);
    int nHeightOfRight = GetHeightOfNode(pUnbalanceNode->m_pRightNode);
    pUnbalanceNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

    nHeightOfLeft = GetHeightOfNode(pLeftNode->m_pLeftNode);
    nHeightOfRight = GetHeightOfNode(pLeftNode->m_pRightNode);
    pLeftNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft,nHeightOfRight)+1;

    //注意:我将叶节点高度视为0,所以上面计算高度时要加1
    return true;
}


//************************************************************************
// 函数名称:  CAVLTree<T>::SingleLeftRotate
// 函数功能:  左单旋
// 所属权限:  private 
// 返回的值:  bool
// 函数参数:  PBINNODE<T> pUnbalanceNode:失衡节点
// 函数参数:  PBINNODE<T> pRightNode:失衡节点的右孩子
// 注意事项: 
//************************************************************************
template<typename T>
bool CAVLTree<T>::SingleLeftRotate(PBINNODE<T> pUnbalanceNode, PBINNODE<T> pRightNode)
{
    //判断pLeftNode是否是pUnbalanceNode的右孩子,防止被忽悠
    if (pUnbalanceNode->m_pRightNode != pRightNode)
    {
        return false;
    }

    PBINNODE<T> pParentOfUnbalance = pUnbalanceNode->m_pParentNode;

    if (pParentOfUnbalance != nullptr)
    {
        //失衡节点不是二叉树的根节点
        if (pParentOfUnbalance->m_pLeftNode == pUnbalanceNode)
        {
            //失衡节点是其父节点的左孩子
            pParentOfUnbalance->m_pLeftNode = pRightNode;
        }
        else
        {
            //失衡节点是其父节点的右孩子
            pParentOfUnbalance->m_pRightNode = pRightNode;
        }

        pRightNode->m_pParentNode = pParentOfUnbalance;
    }
    else
    {
        //失衡节点是二叉树的根节点
        pRightNode->m_pParentNode = nullptr;
        m_pRoot = pRightNode;
    }

    //重置失衡节点为其右孩子的左孩子,并且将失衡节点右孩子先前的左孩子
    //置为失衡节点的右孩子
    pUnbalanceNode->m_pRightNode = pRightNode->m_pLeftNode;
    //失衡节点右孩子先前的左孩子如果存在则将其父节点置为失衡节点
    if (pRightNode->m_pLeftNode != nullptr)
    {
        pRightNode->m_pLeftNode->m_pParentNode = pUnbalanceNode;
    }
    
    //重置失衡节点的父节点为其先前的右孩子
    pUnbalanceNode->m_pParentNode = pRightNode;
    pRightNode->m_pLeftNode = pUnbalanceNode;

    //此时二叉树已经平衡,下面调整pUnbalanceNode和pRightNode的节点高度
    //注意:pUnbalanceNode指向的节点经过调整后不再是失衡子树的根节点,pRightNode
    //    指向的节点经过调整后成为新的平衡子树的根节点,所以这里必须先调整pUnbalanceNode
    //    指向的节点高度,然后在调整pRightNode所指向的节点高度
    int nHeightOfLeft = GetHeightOfNode(pUnbalanceNode->m_pLeftNode);
    int nHeightOfRight = GetHeightOfNode(pUnbalanceNode->m_pRightNode);
    pUnbalanceNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

    nHeightOfRight = GetHeightOfNode(pRightNode->m_pLeftNode);
    nHeightOfLeft = GetHeightOfNode(pRightNode->m_pRightNode);
    pRightNode->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

    return true;
}

//************************************************************************
// 函数名称:  CAVLTree<T>::AdjustTree
// 函数功能:  调整二叉树使其平衡
// 所属权限:  private 
// 返回的值:  bool
// 函数参数:  PBINNODE<T> pInsertNode:待调整的节点
// 注意事项:   
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
    //从pInsertNode节点开始,一直调整至根节点结束
    while (pAdjustPos != nullptr)
    {
        //插入新节点后会导致其他节点的高度发生变化,这里要调整节点高度
        int nHeightOfLeft = GetHeightOfNode(pAdjustPos->m_pLeftNode);
        int nHeightOfRight = GetHeightOfNode(pAdjustPos->m_pRightNode);
        
        //叶节点的高度视为0,所以这里计算节点高度时要加1
        pAdjustPos->m_nHeightOfNode = GetMaxValue(nHeightOfLeft, nHeightOfRight) + 1;

        //根据当前节点的左右子树高度差来判断采用哪种旋转方式来调整
        int nDiffValue = nHeightOfLeft - nHeightOfRight;
        if (nDiffValue < -1)
        {
            //此时当前节点的右子树上插入的新节点导致二叉树不平衡
            pSubNode = pAdjustPos->m_pRightNode;
            if (GetHeightOfNode(pSubNode->m_pLeftNode) > GetHeightOfNode(pSubNode->m_pRightNode))
            {
                //右节点的左子树比右节点的右子树高,所以这里要进行右左双旋
                SingleRightRotate(pSubNode, pSubNode->m_pLeftNode);
                SingleLeftRotate(pAdjustPos, pAdjustPos->m_pRightNode);
            }
            else
            {
                //右节点的左子树比右节点的右子树矮,所以这里要进行左单旋
                SingleLeftRotate(pAdjustPos, pAdjustPos->m_pRightNode);
            }
        }
        else if (nDiffValue > 1)
        {
            pSubNode = pAdjustPos->m_pLeftNode;
            if (GetHeightOfNode(pSubNode->m_pLeftNode) > GetHeightOfNode(pSubNode->m_pRightNode))
            {
                //左节点的左子树比左节点的右子树高,所以这里要进右单旋
                SingleRightRotate(pAdjustPos, pAdjustPos->m_pLeftNode);
            }
            else
            {
                //左节点的左子树比左节点的右子树矮,所以这里要进行左右双旋
                SingleLeftRotate(pSubNode, pSubNode->m_pRightNode);
                SingleRightRotate(pAdjustPos, pAdjustPos->m_pLeftNode);
            }
        }
        else
        {
            //当前节点没有失衡无需调整,则判断其父节点是否需要调整
            pAdjustPos = pAdjustPos->m_pParentNode;
        }
    }
    return true;
}


//************************************************************************
// 函数名称:  CAVLTree<T>::DeleteNode
// 函数功能:  移除节点
// 所属权限:  public 
// 返回的值:  bool:成功返回true,否则返回false
// 函数参数:  T ValueOfNode:要删除的节点值
// 函数参数:  CDeleteNodeFunction<T> & FuncOfDel:用于释放节点内存的函数对象
//************************************************************************
template<typename T>
bool CAVLTree<T>::DeleteNode(T ValueOfNode, CDeleteNodeFunction<T> & FuncOfDel)
{
    //先寻找对应节点
    PBINNODE<T> pDeleteNode = SearchNode(ValueOfNode);
    if (pDeleteNode == nullptr)
    {
        return false;
    }

    //找到对应节点,进行删除
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

    //释放节点内存
    FuncOfDel(pDeleteNode);

    //调整二叉树使其平衡
    AdjustTree(pAdjustPos);
    return bRet;
}



//************************************************************************
// 函数名称:  CAVLTree<T>::SearchNode
// 函数功能:  查找节点
// 所属权限:  public 
// 返回的值:  PBINNODE<T>:没有找到则返回nullptr,否则返回对应节点的指针
// 函数参数:  T Element:节点元素值
// 注意事项:  1.自定义的数据类型T必须要重载>,<这两个运算符
//************************************************************************
template<typename T>
PBINNODE<T> CAVLTree<T>::SearchNode(T Element)
{
    PBINNODE<T> pIndex = m_pRoot;
    while (pIndex != nullptr)
    {
        if (pIndex->m_element > Element)
        {
            //当前节点的元素值比Element大,则向当前节点的左子树查找
            pIndex = pIndex->m_pLeftNode;
            continue;
        }

        if (pIndex->m_element < Element)
        {
            //当前节点的元素值比Element大,则向当前节点的右子树查找
            pIndex = pIndex->m_pRightNode;
            continue;
        }

        break;
    }

    return pIndex;
}
