#include <iostream>
#include <string>
#include <tuple>
using namespace std;

// 'a'의 개수가 n, 'z'의 개수가 m인 문자열에서 k번째 문자열의 첫 글자를 구한다.
// --- 반환값(튜플) ---
// 첫 번째 원소: 알아낸 첫 글자
// 두 번째 원소: 첫 글자가 'a'인 경우는 0이다. 'z'인 경우, 첫 글자가 'a'였을 때
//               가능한 모든 경우의 수를 나타낸다.
//               첫 글자를 배제한 후, 다음 글자를 알아낼 때
//               k의 값을 조정하기 위함이다.
tuple<char, int> GuessFirstChar(int n, int m, int k) {
    // n이나 m의 개수 중 하나라도 0이면, 앞 글자는 정해질 수 밖에 없다.
    if (n == 0)
        return make_tuple('z', 0);
    if (m == 0)
        return make_tuple('a', 0);

    // res의 결과값은, 문자열의 앞이 'a'라고 가정했을 때
    // 가능한 모든 경우의 수이다.
    long long res = 1; 
    
    // 앞 글자가 'a'라고 가정했으므로, n의 수를 하나 줄인다.
    n -= 1;
    
    // 오버플로의 위험이 있으니, res를 조금씩 곱해나가며
    // 계산하고, k가 res보다 작거나 같게 되면 바로 계산을 마친다.
    int len = n + m;
    for (int child = n + 1; child <= len; child++) {
        int parent = 1 + (child - (n + 1));
    
        res *= child;
        res /= parent;

        // k가 res보다 작거나 같은 경우, 앞자리는 무조건 'a'이다.
        if (res >= k)
            return make_tuple('a', 0);
    }
    
    // k가 res보다 큰 경우, 앞자리는 'z'이다.
    return make_tuple('z', (int)res);
}

// 'a'의 개수가 n, 'z'의 개수가 m인 문자열에서 k번째 문자열을 구한다.
string GuessWord(int n, int m, int k) {
    string ret;
    
    int len = n + m;
    for (int i = 0; i < len; i++) {
        char firstChar;
        int diff;

        // 첫 번째 글자를 구한다.
        tie(firstChar, diff) = GuessFirstChar(n, m, k);

        // 매 이터레이션을 마친 후, 첫 번째 글자를 제외한다고 가정하자.
        // 그렇다면 전체 n과 m의 수에서 변동이 발생한다.
        if (firstChar == 'a')
            n -= 1;
        else
            m -= 1;
        
        // 첫 글자가 'z'였던 경우, 이제 k번째 문자열은 첫 글자가 'a'였을 때의
        // 경우의 수을 제외하고 몇 번째 위치인지 다시 조정해야 한다.
        k -= diff;
        
        // 구한 첫 글자는 결과 문자열에 담는다.
        ret.push_back(firstChar);
    }
    
    // k가 아직 1보다 크다면, k가 사전에 있는 문자열 개수보다 많다.
    if (k > 1)
        ret.clear();
    
    return ret;
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    
    string answer = GuessWord(n, m, k);
    if (answer.empty())
        cout << -1;
    else
        cout << answer;
    cout << endl;
    
    return 0;
}
