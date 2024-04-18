#include<bits/stdc++.h>
using namespace std;

int m,t,s;
vector<vector<int> > bat_gen(1e5,vector<int>(1000));
vector<vector<int> > task_arrv(1e5,vector<int>(1000));
vector<vector<int> > assigned_task(1e5,vector<int>(1000,0));

int root_cube(int num){
    int st=1;
    while(pow(st,3)<=num){
        st++;
    }
    return st-1;
}



void task_scheduler(){
    cout<< "********* Task Scheduling Begins *********\n";
    int num_task=1;
    
    while(1){
        bool unchanged = 0;
        for(int i=0;i<t;i++){
            int task_pool=0;
            for(int j=0;j<m;j++){
                task_pool += task_arrv[j][i];
            }
            //cout<<task_pool<<'\n';
            vector<pair<int,int> > bat_cur_slot;
            vector<int > bat_cur_sum(m);
            for(int j=0;j<m;j++){
            for(int ik=i;ik>=0;ik--){
                
            }
            }
            vector<pair<int,int> > bat_pref(m);
            for(int j=0;j<m;j++){
                int sum=0;
                for(int ik=i;ik>=0;ik--){
                    sum+=bat_gen[j][ik];
                }
                bat_pref[j] = make_pair(sum,j);
            }
            for(int j=0;j<m;j++){
                bat_cur_slot.push_back(make_pair(bat_gen[j][i],j));
            }
            sort(bat_cur_slot.begin(),bat_cur_slot.end());
            sort(bat_pref.begin(),bat_pref.end());
            for(int j=0;j<m;j++){
                int req_power = num_task*num_task*num_task;
                //int ind=bat_cur_slot[j].second;
                int ind=bat_pref[j].second;
                int act_pow = req_power-pow(assigned_task[ind][i],3);
                if(act_pow<=bat_gen[ind][i]){
                    if(task_pool>=assigned_task[ind][i]+1){
                    bat_gen[ind][i] -= act_pow;
                    assigned_task[ind][i]+=1;
                    task_pool-=assigned_task[ind][i];
                    unchanged=1;
                    }
                }
                else{
                    int sum=0;
                    bool ok=0;
                    vector<pair<int,int> > taken;
                    for(int k=i;k>=0;k--){
                        if(sum+bat_gen[ind][k]>=act_pow){
                            ok=1;
                            taken.push_back(make_pair(k,act_pow-sum));
                            break;
                        }
                        else{
                            taken.push_back(make_pair(k,bat_gen[ind][k]));
                        }
                    }
                    if(ok){
                        if(task_pool>=assigned_task[ind][i]+1){
                        for(auto [a,b] : taken){
                            bat_gen[ind][a] -= b;
                        }
                        assigned_task[ind][i]+=1;
                        task_pool-=assigned_task[ind][i];
                        unchanged=1;
                        }
                    }
                }
            }
        }
        num_task++;
        if(!unchanged){
            break;
        }
    }
    int total_tasks=0;
    for(int i=0;i<m;i++){
        for(int j=0;j<t;j++){
            cout<<"Server "<<i+1<<" on "<<"slot "<<j+1<<" executed "<<assigned_task[i][j]<<" tasks\n";
            total_tasks+=assigned_task[i][j];
        }
    }
    cout<<"Total number of executed tasks is "<<total_tasks<<'\n';
}

void generate_random_test_cases(int n){
    for(int i=0;i<n;i++){
        m= rand()%100 + 1;
        t=rand()%100+1;
        s=rand()%200 + 50;
        for(int j=0;j<m;j++){
            for(int k=0;k<t;k++){
                bat_gen[j][k]=rand()%s;
                task_arrv[j][k]=rand()%7;
            }
        }
        task_scheduler();
    }
}

signed main(){
    #ifdef LOCAL
    freopen("input.txt", "r" , stdin);
    freopen("output.txt", "w", stdout);
    #endif
    int choice;
    cout<<"Enter 1 to generate random test cases or enter 2 for manual testing\n";
    cin>>choice;
    if(choice == 2){
        cin>>m>>t>>s;
        for(int i=0;i<m;i++){
            for(int j=0;j<t;j++){
                cin>>bat_gen[i][j];
                cin>>task_arrv[i][j];
            }
        }
        task_scheduler();
    }
    else if(choice == 1){
        int num_count;
        cout<<"Enter the number of test cases to generate\n";
        cin>>num_count;
        generate_random_test_cases(num_count);
    }
    else{
        cout<<"Wrong Command entered, Exiting ...\n";
    }
    #ifdef LOCAL
    clock_t tStart = clock();
    cerr<<fixed<<setprecision(10)<<"\nTime Taken: "<<(double)(clock()- tStart)/CLOCKS_PER_SEC<<endl;
    #endif
}