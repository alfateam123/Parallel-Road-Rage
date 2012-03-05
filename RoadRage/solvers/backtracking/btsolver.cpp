#include <iostream>
#include "btsolver.h"

#ifdef _OPENMP
#include <omp.h> //OpenMP routines
#endif
using namespace std;


BTSolver::BTSolver() : Solver()
{
    this->acceptConstraints=new QVector<Constraint*>;
    this->rejectConstraints=new QVector<Constraint*>;
}

void BTSolver::getSolutions(Path startSolution, QVector<Path> *solutions)
{
    //QVector<Path>* solutions=new QVector<Path>();
    QStack<Path> *candidateSolutionsStack=new QStack<Path>();

    //initialize stack of candidate solutions with a solution
    //that could be empty of partially fulled.
    candidateSolutionsStack->push(startSolution);
    unsigned int solutionCounter=0;
    //find solutions
    Path actualCandidate=NULL;
    QVector<Path> *nexts;

#pragma omp parallel private(actualCandidate, nexts) num_threads(2)
{

    #pragma omp master
    {
        //useful debug options...
        cout<<"# max threads: "<<omp_get_max_threads()<<endl;
        cout<<"# threads: "<<omp_get_num_threads()<<endl;
    }
    do
    {
        cout<<"dentro! "<<candidateSolutionsStack->size()<<endl;
        //get first element of the stack w/o deleting it

        //Path actualCandidate=candidateSolutionsStack->top();
        //candidateSolutionsStack->pop(); //delete first element of the stack


       #pragma omp critical
       {
           cout<< omp_get_thread_num() <<"retrieving a solution..."<<endl;
           if(!candidateSolutionsStack->isEmpty())
           {
               cout<<omp_get_thread_num()<<"retrieved!"<<endl;
               actualCandidate=candidateSolutionsStack->top();
               candidateSolutionsStack->pop();
           }
           else
           {
               actualCandidate=NULL;
           }
       }



        if(!this->rejectSolution(actualCandidate))
        {
            //cout<<"ok, this solution is interesting..."<<endl;
            if(!this->acceptSolution(actualCandidate))
            {
                //now generate next candidate solutions
//                cout<<omp_get_thread_num()<<"adding new solutions..."<<endl;
                #pragma omp critical
                {
                    nexts=new QVector<Path>();
                    actualCandidate.nextCandidates(nexts);
//                    cout<<omp_get_thread_num()<<"-"<<nexts->size()<<" candidate solutions are been created!"<<endl;

                for(int i=0; i<nexts->size(); i++)
                {
                    if(nexts->isEmpty() && nexts->size()==0)
                        cout<<"wat?"<<endl;
                    candidateSolutionsStack->push(nexts->at(i));
                }
                nexts=NULL; //free space of next candidate solutions
                }   //end #pragma omp critical
             }

            else
            {
                //save solution
                actualCandidate.print();
                //cout<<"found a solution!"<<endl;
                //cout<<"actual solutions: "<<solutions->size()+1<<endl;
                solutionCounter++;
                #pragma omp critical
                {
                    actualCandidate.print();
                    solutions->append(actualCandidate);
                }
            }
        }
        else
        {
            /*cout<<"rejected solution!"<<endl;
            for(int pos=0; pos<actualCandidate.getPath().size();
                pos++)
                cout<<actualCandidate.getPath().at(pos);
            cout<<endl;*/
        }
    }while(!candidateSolutionsStack->isEmpty());
} //end #pragma omp parallel
    cout<<"fuori"<<endl;
    //freein' memory
    delete candidateSolutionsStack;
    cout<<"solutions->size()=="<<solutions->size()<<" solutionCounter="<<solutionCounter<<endl;
    //return solutions;
}

/*
bool BTSolver::rejectSolution(Path candidate)
{
    bool isRejected=false;
#pragma omp parallel for
    for(int i=0; i<this->rejectConstraints->size(); i++)
    {
        //this->rejectConstraints->at(i)->printName();
        if(!this->rejectConstraints->at(i)->isRespected(candidate))
        {
            //cout<<"not respected constraint at "<<i<<"th position!"<<endl;
            isRejected=true;
        }
    }
    return isRejected;
}

bool BTSolver::acceptSolution(Path candidate)
{
    bool isAccepted=false;
#pragma omp parallel for
    for(int i=0; i<this->acceptConstraints->size(); i++)
    {
        if(this->acceptConstraints->at(i)->isRespected(candidate))
            isAccepted=true;
    }
    return isAccepted;
}

void BTSolver::addAcceptConstraint(Constraint *newConstraint)
{this->acceptConstraints->push_back(newConstraint);}

void BTSolver::addRejectConstraint(Constraint *newConstraint)
{this->rejectConstraints->push_back(newConstraint);}
*/