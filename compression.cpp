#include "compression.h"

int binarystringtoint(string binarystring)
{
    int sum = 0;
    for (int i = 0; i < binarystring.size(); i++)
    {
        if (binarystring[i] == '1')
        {
            int j = pow(2, binarystring.size() - i - 1);
            sum += j;
        }
    }
    return sum;
}

string inttobinarystring(int value)
{
    string binarystring;
    while (value > 0)
    {
        int r = value % 2;
        if (r == 1)binarystring.insert(0, 1, '1');
        else binarystring.insert(0, 1, '0');
        value = value / 2;
    }
    if (binarystring.size() < 8)
    {
        binarystring.insert(0, 8 - binarystring.size(), '0');
    }
    return binarystring;
}

bool compare(HuffmanTreeNode* node1, HuffmanTreeNode* node2)
{
    return node1->Weight.second < node2->Weight.second;
}



Compression::Compression(QWidget *parent) : QMainWindow(parent)
{

}


void Compression::DEL(HuffmanTreeNode*& root)
{
    if(root==NULL) return;
    DEL(root->leftson);
    DEL(root->rightson);
    delete root;
}

void Compression:: BinaryString_Init()
{
    for (int i = 0; i < source_string.size(); i++)
    {
        binary_string+= passwordmap[source_string[i]];
    }
}

void Compression::Weightmap_Init(QFile& in)
{
//    qDebug() << "222";
    QFileInfo fileInfo(in.fileName());
    fileExtension = fileInfo.suffix();
    qDebug() << fileExtension;


    QByteArray a;
    while (!in.atEnd())
    {
        a=in.read(1024);
        string b=a.toStdString();
//        qDebug() << b.length();
        for(int i=0;i<b.size();i++)
        {
            unsigned char ch=b[i];
            source_string += ch;
            weightmap[ch]++;
        }
    }
}

HuffmanTreeNode* Compression::MakehuffmanTreeNode(int i)
{
    HuffmanTreeNode* huffman = new HuffmanTreeNode;
    huffman->Weight.first = i;
    huffman->Weight.second = weightmap[i];
    huffman->tag = 0;
    huffman->leftson = NULL;
    huffman->rightson = NULL;
    return huffman;
}

void Compression::HuffmanTreeVector_Init()
{
    for (map<unsigned char, int>::iterator it = weightmap.begin(); it != weightmap.end(); it++)
    {
        HuffmanTreeNode* huffman = MakehuffmanTreeNode(it->first);
        container.push_back(huffman);
    }
}

void Compression::HuffmanTree_Init()
{
    while (container.size() != 1)
    {
        sort(container.begin(), container.end(), compare);
        int sum = container[0]->Weight.second + container[1]->Weight.second;
        HuffmanTreeNode* newhuffman = new HuffmanTreeNode;
        newhuffman->Weight.second = sum;
        newhuffman->Weight.first = 0;
        newhuffman->tag = 1;
        newhuffman->leftson = container[0];
        newhuffman->rightson = container[1];
        container.erase(container.begin());
        container.erase(container.begin());
        container.push_back(newhuffman);
    }
}

void Compression::ZipPassword_Init(HuffmanTreeNode* &root, string& password)
{
    if (root != NULL && !root->tag)
    {
        root->zippassword = password;
        passwordmap[root->Weight.first] = password;
    }
    if (root->leftson != NULL)
    {
        ZipPassword_Init(root->leftson, password += "0");
        password.pop_back();
    }
    if (root->rightson != NULL)
    {
        ZipPassword_Init(root->rightson, password+="1");
        password.pop_back();
    }

    // 在这里插入扩展名信息
//    if (root == container[0]) {
////        root->zippassword += fileExtension.toStdString();
////        passwordmap[256] = fileExtension.toStdString();
////        qDebug() << root->zippassword.toUtf8();

//         qDebug() << fileExtension.toUtf8();
//    }

//    if (root == container[0]) {
////        root->zippassword += fileExtension.toStdString();
////        passwordmap[256] = fileExtension.toStdString();  // 使用 256 或其他独特的值
//        qDebug() << QString(root->zippassword[2]);
//    }

//    if (root == container[0]) {
//        QString extensionInfo = fileExtension;
//        out.writeRawData(extensionInfo.toUtf8().constData(), extensionInfo.size());
//    }
}

void Compression::zip(QString path, QString path2)
{
    clock_t begin=clock();
    QFile openfile(path);
    if(!openfile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,QString("警告"),QString("文件打开失败"));
        emit error();
        return;
    }
    Weightmap_Init(openfile);
    emit mysignal(10);
    HuffmanTreeVector_Init();
    emit mysignal(20);
    HuffmanTree_Init();
    emit mysignal(30);
    string empty="";
    ZipPassword_Init(container[0],empty);
    emit mysignal(40);
    BinaryString_Init();
    emit mysignal(50);


//    qDebug() << path;
    int len1 = fileExtension.length();
//    qDebug() << len1;

    path.chop(len1 + 1);
//    qDebug() << path;

//    qDebug() << "123";
//    qDebug() << fileExtension;

    path+=".huf";

    qDebug() << path;

    name=path.mid(path.lastIndexOf("/"));
    qDebug() << name;

    openfile.close();
    QFile savefile(path2 + name);
    savefile.open(QIODevice::WriteOnly);
    QDataStream out(&savefile);
    int size = passwordmap.size();
    if (size == 256) size = 0;
    int length = 0;
    out<<size;
    length++;
    double k=1;
    for (map<unsigned char, string>::iterator it = passwordmap.begin(); it != passwordmap.end(); it++)
    {
        emit mysignal(50+double(25*k++)/passwordmap.size());
        int first = it->first;
        out<<first;
//        qDebug() << first;
        length++;
        string second = it->second;
//        std::cout << second;
//        qDebug() << QString(second);
//        QDebug() << second.toUtf8();
//        QString aaa = second;
//        qDebug() << aaa;
//        QString myQString = QString::fromStdString(second);
//        qDebug() << myQString;
        int size = second.size();
        out<<size;
        length++;
        int n = 8 - second.size() % 8;
        if (n)
        {
            second.append(n, '0');
        }
        for (int i = 0; i < second.size(); i += 8)
        {
            string k = second.substr(i, 8);
            int temp = binarystringtoint(k);
            unsigned char ch = temp;
            out<<ch;
            length++;
        }
    }
    int n = 8 - binary_string.size() % 8;
    if (n)
    {
        binary_string.append(n, '0');
    }
    length++;
    int totalbitsize = binary_string.size() / 8;
    for (int i = 0; i < binary_string.size(); i += 8)
    {
        emit mysignal(75+double(25*i)/binary_string.size());
        string k = binary_string.substr(i, 8);
        int temp = binarystringtoint(k);
        unsigned char ch = temp;
        out<<ch;
        length++;
    }
    unsigned char temp=n;
    out<<temp;
    length++;
    emit mysignal(100);
    int newlength=savefile.size();
    savefile.close();
    clock_t end=clock();
//    QString tip("理论压缩比:");
//    tip+=QString::number(double(totalbitsize * 100) / source_string.size());
//    tip+="%,实际压缩比：";
//    tip+=QString::number(double(newlength * 100) / source_string.size());
//    tip+="%,压缩用时：";
//    tip+=QString::number(double(end-begin)/CLOCKS_PER_SEC);
//    tip+="s";
    tip+="压缩比：";
    tip+=QString::number(double(newlength * 100) / source_string.size());
    tip+="%,压缩用时：";
    tip+=QString::number(double(end-begin)/CLOCKS_PER_SEC);
    tip+="s";
    tip+="\n";


//    QMessageBox::about(this,"压缩说明",tip);
    weightmap.clear();
    passwordmap.clear();
    source_string.clear();
    binary_string.clear();
    DEL(container[0]);
    container.clear();
}

void Compression::unzip(QString path, QString path2)
{
    clock_t begin=clock();
    if(path.right(4)!=".huf")
    {
        QMessageBox::information(NULL,QString("警告"),QString("此文件非哈夫曼压缩文件，打开失败"));
        emit error();
        return;
    }
    QFile openfile(path);
    if(!openfile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,QString("警告"),QString("文件打开失败"));
        emit error();
        return;
    }
    QDataStream in(&openfile);
    int zipmapsize;
    in>>zipmapsize;
    if (zipmapsize == 0) zipmapsize = 256;
    for (int i = 1; i <= zipmapsize ; i++)
    {
        int zipkey;
        in>>zipkey;
        int valuelength;
        in>>valuelength;
        string valuestring;
        for (int i = 1; i <= valuelength / 8 + 1; i++)
        {
            emit mysignal(double(20*i)/(valuelength/8 +1));
            unsigned char ch;
            in>>ch;
            int zipvalue=ch;
            valuestring += inttobinarystring(zipvalue);
        }
        valuestring.erase(valuelength, valuestring.size() - valuelength + 1);



        zippassword[valuestring] = zipkey;
//        qDebug() << zipkey;
//        qDebug() << passwordmap[256];

        // 检查是否是扩展名信息
//        if (zipkey == 256) {
//            fileExtension = QString::fromStdString(valuestring);
//            qDebug() << fileExtension;
//            continue;  // 不处理扩展名信息，继续读取下一项
//        }

//        if (zipkey == 256) {  // 使用相同的特殊值
//            fileExtension = QString::fromStdString(valuestring);
//            qDebug() << fileExtension;
//            continue;  // 不处理扩展名信息，继续读取下一项
//        }
    }
//    zipkey = 256;
//    qDebug() << fileExtension;

    string zipstring;
    while (!in.atEnd())
    {
        unsigned char ch;
        in>>ch;
        int bit=ch;
        zipstring += inttobinarystring(bit);
    }
    emit mysignal(50);
    int zerosize = binarystringtoint(zipstring.substr(zipstring.size() - 8));
    zipstring.erase(zipstring.size() - zerosize - 8, zerosize + 8);
    openfile.close();
    path.chop(4);
    qDebug() << "123";
    qDebug() << path;
    path.append("." + fileExtension);
    qDebug() << path;
    path.insert(path.lastIndexOf('.'),"(New)");

    name=path.mid(path.lastIndexOf("/"));
    qDebug() << name;

    QFile savefile(path2+name);
    savefile.open(QIODevice::WriteOnly);
    string str;
    for (int i = 0; i < zipstring.size(); i++)
    {
        emit mysignal(50+double(50*i)/zipstring.size());
        str += zipstring[i];
        map<string, int>::iterator it = zippassword.find(str);
        if (it != zippassword.end())
        {
            unsigned char temp=it->second;
            savefile.write(reinterpret_cast<char*>(&temp),1);
            str.clear();
        }
    }
    emit mysignal(100);
    savefile.close();
    clock_t end=clock();
    QString tip="解压用时:";
    tip+=QString::number(double(end-begin)/CLOCKS_PER_SEC);
    tip+="s";
//    QMessageBox::about(this,"解压说明",tip);
    zippassword.clear();
}

QString Compression::getTip(){
    return tip;
}
