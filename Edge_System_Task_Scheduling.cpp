#include<bits/stdc++.h>
using namespace std;
#include <fstream>


int m,t,s;
vector<vector<int> > bat_gen(1e5,vector<int>(1000));
vector<vector<int> > task_arrv(1e5,vector<int>(1000));
vector<vector<int> > assigned_task(1e5,vector<int>(1000,0));
vector<int> battery_total(1e5);

int root_cube(int num){
    int st=1;
    while(pow(st,3)<=num){
        st++;
    }
    return st-1;
}

void writeCSV(const std::string& filename, const std::vector<std::vector<int> > data) {
    vector<float> bat_rem(m,0);
    for(int i=0;i<m;i++){
        for(int j=0;j<t;j++){
            bat_rem[i]+=bat_gen[i][j];
        }
    }
    // Open the file for writing (create if not exists)
    std::ofstream file(filename, std::ios::out);

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    for(int i=0;i<t;i++){
        file<<i+1;
        if(i<t-1){
            file<<",";
        }
        else{
            file<<'\n';
        }
    }
    // Iterate over each row of data
    for (int i=0;i<m;i++) {
        // Iterate over each element in the row
        for (int j = 0; j < t; j++) {
            // Write the element to the file
            file << data[i][j];

            // Add a comma after each element, except the last one
            if (j < t - 1) {
                file << ",";
            }
        }
        // Add a new line after each row
        file << std::endl;
    }

    // Close the file
    file.close();

    std::ofstream file1(filename, std::ios::out);

    // Check if the file opened successfully
    if (!file1.is_open()) {
        std::cerr << "Error opening file: " << filename<<"battery" << std::endl;
        return;
    }
    for(int i=0;i<t;i++){
        file<<i+1;
        if(i<t-1){
            file<<",";
        }
        else{
            file<<'\n';
        }
    }
    // Iterate over each row of data
    for (int i=0;i<m;i++) {
        // Iterate over each element in the row
        for (int j = 0; j < t; j++) {
            // Write the element to the file
            file << data[i][j];

            // Add a comma after each element, except the last one
            if (j < t - 1) {
                file << ",";
            }
        }
        // Add a new line after each row
        file << std::endl;
    }

    // Close the file
    file.close();
}

void task_scheduler(){
    cout<< "********* Task Scheduling Begins *********\n";
    int num_task=1;
    for(int i=0;i<m;i++){
        battery_total[i]=0;
        for(int j=0;j<t;j++){
            battery_total[i]+=bat_gen[i][j];
        }
    }
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
        m= rand()%20 + 1;
        t=rand()%24+1;
        s=rand()%20 +5;
        for(int j=0;j<m;j++){
            for(int k=0;k<t;k++){
                bat_gen[j][k]=rand()%s;
                cout<<"Server "<<j<<" generate "<<bat_gen[j][k]<<" at "<<k<<" slot\n";
                task_arrv[j][k]=rand()%7;
                cout<<"Server "<<j<<" tasks "<<task_arrv[j][k]<<" arrive at "<<k<<" slot\n";
            }
        }
        task_scheduler();
        string filename="data"+to_string(i+1)+".csv";
        writeCSV(filename,assigned_task);
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
        string filename="data.csv";
        writeCSV(filename,assigned_task);
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