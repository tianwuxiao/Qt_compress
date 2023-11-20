#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <QMainWindow>
#include<QMessageBox>
#include<QFile>
#include<QDataStream>
#include<QString>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<ctime>
#include<QDebug>
#include<QFile>
#include<QFileInfo>

using namespace std;

struct HuffmanTreeNode
{
    pair<unsigned char, int>Weight;
    string zippassword;
    HuffmanTreeNode* leftson;
    HuffmanTreeNode* rightson;
    bool tag;
};


class Compression : public QMainWindow
{
    Q_OBJECT
public:
    explicit Compression(QWidget *parent = nullptr);
    void zip(QString path, QString path2);
    void unzip(QString path, QString path2);
    QString getTip();
protected:
    void DEL(HuffmanTreeNode*& root);
    void BinaryString_Init();
    void Weightmap_Init(QFile& in);
    HuffmanTreeNode* MakehuffmanTreeNode(int i);
    void HuffmanTreeVector_Init();
    void HuffmanTree_Init();
    void ZipPassword_Init(HuffmanTreeNode* &root, string& password);
private:
    map<unsigned char, int> weightmap;
    map<unsigned char, string>passwordmap;
    string source_string;
    string binary_string;
    vector<HuffmanTreeNode* > container;
    map<string, int> zippassword;
    QString fileExtension;
    QString name;
    QString tip;
signals:
    void error();
    void mysignal(double per);
public slots:
};

#endif // COMPRESSION_H

