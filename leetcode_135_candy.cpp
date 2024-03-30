// My solution to LeetCode problem "135. Candy"
// https://leetcode.com/problems/candy/description/?envType=study-plan-v2&envId=top-interview-150
// Analytic
// Find the first longest decreasing segment, then hills (of left valley, peak, right valley). Finally the last segment.
// (P1) Indicators : i_up (monotone increasing), i_down (monotone decreasing), i_peak (the peak of a hill), i_valley_r (the right valley), i_dup_near_peak_l (duplicated elements near the peak to the left), i_dup_near_peak_r (duplicated elements near the peak to the right)
// (P2) Index : id_l (index of left valley), id_m (index of the peak), id_r (index of right valley), id_dup_l (index of duplicated element nearest to the peak to the left), id_dup_r (index of duplicated element nearest to the peak to the right)
// Submitted to LeetCode and passed all tests with 10ms runtime (beats 90.58% of users with c++) and 19.98MB memory (beats 75.81 of users with c++) 

#include <iostream>
#include <vector>
// #mark a#

using namespace std;

class Solution {
public:
  int candy(vector<int>& ratings) {
    // #mark b#
    int s=ratings.size();
    if(s==1){ return 1; }
    vector<int> spot(s, 1);
    int sum=0, n_df_l=0;
    int id_l=0, id_m=0, id_r=0;
    int a, b;
    a=ratings[0];
    // find the first decreasing segment
    for(int i=1;i<s;i++){
      b=ratings[i];
      if(b>a){
        id_l=i-1;
        break;
      } else if(b==a){
        spot[i]=2;
        id_l=i;
      } else {
        id_l=i;
      }
      a=b;
    }
    // #mark f#
    sum+= sum_dec ( ratings, s, spot, 0, id_l, 1);
    // cout << "sum " << sum << "\n";
    int valley_l=ratings[id_l];
    // indicator of increasing, decreasing, peak, valley to the right of peak
    bool i_up=0, i_peak=0, i_down=0, i_valley_r=0;
    // indicator of duplicated elements near peak, from left and right
    bool i_dup_near_peak_l=0, i_dup_near_peak_r=0;
    // index of duplicated element nearest to peak, from left and right
    int id_dup_l, id_dup_r;
    a=ratings[id_l];
    for(int i=id_l+1;i<s;i++){
      b=ratings[i];
      if(b>a){ 
        // #mark g# ========================= b>a ==========================
        if(i_down && i_peak){
          i_valley_r=0;
          id_r=i-1;
          sum+=sum_hill( ratings, s, spot, id_dup_l, id_dup_r, i_dup_near_peak_l, i_dup_near_peak_r, id_l, id_m, id_r);
          // cout << "sum " << sum << "\n";
          // #mark j#
          i_peak=0;
          i_dup_near_peak_l=0; i_dup_near_peak_r=0;
          id_l=id_r;
        } else{
          id_m=i;
        }
        if(i==s-1 && i_peak==0){
          // cout << "The end. Increasing\n";
          sum+= sum_inc ( ratings, spot, id_l+1, s-1, 2);
          // cout << "sum " << sum << "\n";
        }
        i_up=1; i_down=0;
      } else if(b==a){
        // #mark l# ========================= b==a ==========================
        if(i_up){
          i_dup_near_peak_l=1;
          id_dup_l=i;
        } else if (i_down){
          if(i_dup_near_peak_r==0){
            i_dup_near_peak_r=1;
            id_dup_r=i-1;
          }
        }
        spot[i]=2;
        if(i==s-1 && i_peak==1){
          id_r=s-1;
          sum+=sum_hill( ratings, s, spot, id_dup_l, id_dup_r, i_dup_near_peak_l, i_dup_near_peak_r, id_l, id_m, id_r);
          // cout << "sum " << sum << "\n";
        } else if(i==s-1 && i_peak==0 ){
          // cout << "The end. Increasing\n";
          sum+= sum_inc ( ratings, spot, id_l+1, s-1, 2);
          // cout << "sum " << sum << "\n";
        }
      } else {
        // #mark p# ========================= b<a ==========================
        if(i_up){
          i_peak=1;
          id_m=i-1;
        }
        i_down=1; i_up=0;
        id_r=i;
        if(i==s-1 && i_peak==1){
          // cout << "The end. Decreasing. Last hill\n";
          sum+=sum_hill( ratings, s, spot, id_dup_l, id_dup_r, i_dup_near_peak_l, i_dup_near_peak_r, id_l, id_m, id_r);
          // cout << "sum " << sum << "\n";
          // i_valley_r=1;
          // id_r=s-1;
        }
      }
      a=b;
    }
    return sum;
  }
  // #mark t# ================ function sum_hill ============================
  int sum_hill(const vector<int> & ratings, const int s, const vector<int> & spot, const int id_dup_l, const int id_dup_r, const int i_dup_near_peak_l, const int i_dup_near_peak_r, const int id_l, const int id_m, const int id_r){
    // cout << "id_l " << id_l << "\n";
    // cout << "id_m " << id_m << "\n";
    // cout << "id_r " << id_r << "\n";
    // cout << "i_dup_near_peak_l " << i_dup_near_peak_l << "\n";
    // cout << "i_dup_near_peak_r " << i_dup_near_peak_r << "\n";
    // cout << "id_dup_l " << id_dup_l << "\n";
    // cout << "id_dup_r " << id_dup_r << "\n";
    int sum=0;
    if(i_dup_near_peak_l==1 && i_dup_near_peak_r==1){
      // sum candies near peak
      int inc_l=id_m-id_dup_l, inc_r=id_dup_r-id_m;
      int inc_max=max(inc_l, inc_r);
      int inc_min=min(inc_l, inc_r);
      for(int i=1;i<=1+inc_max;i++){
        sum+=i;
      }
      for(int i=1;i<=inc_min;i++){
        sum+=i;
      }
      // sum candies from left valley (excludingly) to near peak
      int cdy=2;
      for(int i=id_l+1;i<=id_dup_l-1;i++){
        if(spot[i]==2){ cdy=1; }
        sum+=cdy;
        cdy++;
      }
      // sum candies from right valley (includingly) to near peak
      cdy=1;
      for(int i=id_r;i>=id_dup_r+1;i--){
        if(i+1<=s-1 && spot[i+1]==2){ cdy=1; }
        sum+=cdy;
        cdy++;
      }
    } else if(i_dup_near_peak_l==0 && i_dup_near_peak_r==1){
      // sum candies near peak
      int inc_l=id_m-id_l, inc_r=id_dup_r-id_m;
      int inc_max=max(inc_l, inc_r);
      int inc_min=min(inc_l, inc_r);
      for(int i=1;i<=1+inc_max;i++){
        sum+=i;
      }
      for(int i=1;i<=inc_min;i++){
        sum+=i;
      }
      sum--;    // avoid summing the 1 in left valley twice
      // sum candies from right valley (includingly) to near peak
      int cdy=1;
      for(int i=id_r;i>=id_dup_r+1;i--){
        if(i+1<=s-1 && spot[i+1]==2){ cdy=1; }
        sum+=cdy;
        cdy++;
      }
    } else if(i_dup_near_peak_l==1 && i_dup_near_peak_r==0){
      // sum candies near peak
      int inc_l=id_m-id_dup_l, inc_r=id_r-id_m;
      int inc_max=max(inc_l, inc_r); 
      int inc_min=min(inc_l, inc_r);
      for(int i=1;i<=1+inc_max;i++){ 
        sum+=i;
      }
      for(int i=1;i<=inc_min;i++){
        sum+=i;
      }
      // sum candies from left valley (excludingly) to near peak
      int cdy=2;
      for(int i=id_l+1;i<=id_dup_l-1;i++){
        if(spot[i]==2){ cdy=1; }
        sum+=cdy;
        cdy++;
      }
    } else if(i_dup_near_peak_l==0 && i_dup_near_peak_r==0){
      int inc_l=id_m-id_l, inc_r=id_r-id_m;
      int inc_max=max(inc_l, inc_r); 
      int inc_min=min(inc_l, inc_r);
      for(int i=1;i<=1+inc_max;i++){ 
        sum+=i;
      }   
      for(int i=1;i<=inc_min;i++){
        sum+=i;
      } 
      sum--;  // avoid summing the 1 in left valley twice
    }
    return sum;
  }
  // #mark v# ================ function sum_inc ============================
  int sum_inc (const vector<int> & ratings, const vector<int> & spot, const int a, const int b, const int cdy){
    // cdy is the number of candy at index "a"
    int sum=0;
    int c=cdy;
    for(int i=a;i<=b;i++){
      if(spot[i]==2){ c=1; }
      sum+=c;
      c++;
    }
    return sum;
  }
  // #mark w# ================ function sum_dec ============================
  int sum_dec (const vector<int> & ratings, const int s, const vector<int> & spot, const int a, const int b, const int cdy){
    // cdy is the number of candy at index "b"
    int sum=0;
    int c=cdy;
    for(int i=b;i>=a;i--){
      if(i+1<=s-1 && spot[i+1]==2){ c=1; }
      sum+=c;
      c++;
    }
    return sum;
  }
};

// #mark z#

int main(){
  // #mark w#
  Solution sol;
  // vector<int> ratings={0, 1, 2, 2, 3, 4};
  // int sum=sol.candy(ratings);
  // vector<int> ratings={4, 3, 2, 2, 0, 1, 1, 2, 2, 3, 1, 1, 1, 0, 2, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 4, 4, 5, 3, 2, 0, 1, 2, 3, 2, 1, 6, 3, 3, 4, 4};  // 71
  // int sum=sol.candy(ratings);
  // vector<int> ratings={4, 3, 2, 0, 1, 2, 2};   // 16
  // int sum=sol.candy(ratings);
  vector<int> ratings={1,2,87,87,87,2,1};   // 13
  int sum=sol.candy(ratings);
  cout << "sum " << sum << "\n";
}

