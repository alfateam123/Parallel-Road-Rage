#ifndef PATH_H
#define PATH_H

//#include <QVector>
#include <QVector>
//#include <QQString>
#include <QString>
#include "iostream"
using namespace std;

class Path
{

private:
    QVector<unsigned int> path;
    QVector<QString> names;
    unsigned int MAX_NODES;

public:
    Path();
    Path(unsigned int maxNodes);
    Path(unsigned int maxNodes, QVector<unsigned int> path);

    QVector<unsigned int> getPath();
    void appendPoint(unsigned int point, bool putAtFirst=false);

    //void nextCandidates(QVector<Path> *nextCandidates);

    void print()
    {
        cout<<"path is: ";
        for(int i=0; i<this->path.size(); i++)
            cout<<this->path.at(i)<<" ";
        cout<<endl;
    }
    int getNodes();

    void addName(QString name);
    QVector<QString> namesRelatedToPath();
    QString getNameByPoint(unsigned int point);
    int getPointByName(QString name);
    void replace(unsigned int position, unsigned int value);
};

#endif // SOLUTION_H
