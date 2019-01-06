// BinTree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AVLTree.h"
#include <iostream>
class MyTraversalFunc :public CTraversalFunction<int>
{
  virtual void operator()(PBINNODE<int>  pNode);
};


void MyTraversalFunc::operator()(PBINNODE<int>  pNode)
{
  std::cout << pNode->m_element <<" ";
}

class MyDeleteFunc :public CDeleteNodeFunction<int>
{
    virtual void operator()(PBINNODE<int> pDelNode);
};

void MyDeleteFunc::operator()(PBINNODE<int> pDelNode)
{
    delete pDelNode;
}


int main()
{
  CAVLTree<int> binTree;

  //BINNODE<int> node1(1);
  //BINNODE<int> node2(2);
  //BINNODE<int> node3(3);
  //BINNODE<int> node4(4);
  //BINNODE<int> node5(5);
  //BINNODE<int> node6(6);
  //BINNODE<int> node7(7);
  //BINNODE<int> node8(8);
  //BINNODE<int> node9(9);
  //BINNODE<int> node10(10);
  //BINNODE<int> node11(11);
  //BINNODE<int> node12(12);


  //node1.m_pLeftNode = &node2;
  //node1.m_pRightNode = &node5;
  //node2.m_pLeftNode = &node3;
  //node2.m_pRightNode = &node4;
  //node3.m_pLeftNode = &node10;
  //node3.m_pRightNode = &node11;
  //node11.m_pLeftNode = &node12;
  //node4.m_pRightNode = &node6;
  //node5.m_pLeftNode = &node7;
  //node5.m_pRightNode = &node9;
  //node7.m_pRightNode = &node8;


  //binTree.m_pRoot = &node1;
  //MyTraversalFunc funcObj;

  //binTree.PostOrderRecursiveTraversal(funcObj, binTree.m_pRoot);
  //std::cout << std::endl;
  //binTree.PostOrderTraversal(funcObj, binTree.m_pRoot);
  //binTree.InOrderRecursiveTraversal(funcObj, binTree.m_pRoot);
  //std::cout << std::endl;
  //binTree.InOrderTraversal(funcObj, binTree.m_pRoot);
  //binTree.PreOrderRecursiveTraversal(funcObj,binTree.m_pRoot);
  //std::cout << std::endl;
  //binTree.PostOrderTraversal(funcObj, binTree.m_pRoot);

  //binTree.PreOrderRecuursiveTraversal(funcObj,binTree.m_pRoot);
  //std::cout << "--------------------" << std::endl;
  //binTree.PreOrderTraversal(funcObj, binTree.m_pRoot);

  //binTree.PostOrderRecursiveTraversal(funcObj, binTree.m_pRoot);
  //std::cout <<"--------------------" << std::endl;
  //binTree.PostOrderTraversal(funcObj, binTree.m_pRoot);

  //binTree.LevelOrderTraversal(funcObj, binTree.m_pRoot);


  /*
  int nIndex = 10;
  while (nIndex > 0)
  {
      PBINNODE<int> pNewNode = new BINNODE<int>(nIndex);
      binTree.InsertNode(pNewNode);
      nIndex;
  }
  */


  /*左右双旋
  BINNODE<int> Node20(20);
  binTree.InsertNode(&Node20);
  BINNODE<int> Node10(10);
  binTree.InsertNode(&Node10);
  BINNODE<int> Node30(30);
  binTree.InsertNode(&Node30);
  BINNODE<int> Node5(5);
  binTree.InsertNode(&Node5);
  BINNODE<int> Node15(15);
  binTree.InsertNode(&Node15);
  BINNODE<int> Node25(25);
  binTree.InsertNode(&Node25);
  BINNODE<int> Node40(40);
  binTree.InsertNode(&Node40);
  BINNODE<int> Node22(22);
  binTree.InsertNode(&Node22);
  BINNODE<int> Node28(28);
  binTree.InsertNode(&Node28);
  BINNODE<int> Node26(26);
  binTree.InsertNode(&Node26);
  */


  //右左双旋
  binTree.InsertNode(20);
  binTree.InsertNode(10);
  binTree.InsertNode(30);
  binTree.InsertNode(5);
  binTree.InsertNode(15);
  binTree.InsertNode(25);
  binTree.InsertNode(40);
  binTree.InsertNode(39);
  binTree.InsertNode(45);
  binTree.InsertNode(38);

  MyDeleteFunc FuncOfDel;
  binTree.DeleteNode(20, FuncOfDel);
  binTree.DeleteNode(10, FuncOfDel);
  binTree.DeleteNode(30, FuncOfDel);
  binTree.DeleteNode(5, FuncOfDel);
  binTree.DeleteNode(25, FuncOfDel);
  binTree.DeleteNode(40, FuncOfDel);
  binTree.DeleteNode(39, FuncOfDel);
  binTree.DeleteNode(45, FuncOfDel);
  binTree.DeleteNode(38, FuncOfDel);

  system("pause");
  return 0;
}

