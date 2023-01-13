#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

map<char, string> alphabet_code;
map<string, char> alphabet_decode;
vector<pair<char, int>> alphabet;
string decode_str = "";

// �������������� ���� ������
struct Node {
    string key; // �������� ����
    Node* parent; // ��������
    Node* left; // ����� "�������"
    Node* right; // ������ "�������"

    // �����������
    Node(string key)
    {
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    };
};


Node* code_tree = new Node(""); // ������� ������


vector<pair<char, int>> make_alphabet_tabel(string str) {
    map<char, int> unsorted_alphabet;
    vector<pair<char, int>> result_alphabet;


    for (int i = 0; i < str.length(); i++)
    {
        if (unsorted_alphabet.count(str[i]) == 0) unsorted_alphabet[str[i]] = 1; // ���� ����� ����������� �������
        else unsorted_alphabet[str[i]]++;
    }


    copy(unsorted_alphabet.begin(), unsorted_alphabet.end(), back_inserter(result_alphabet)); // �������� map � ������ ���
    // ��������� ������ �� ����������
    sort(result_alphabet.begin(), result_alphabet.end(), [](const auto& x, const auto& y) { return x.second > y.second; });
    return result_alphabet;
}

void make_tree(int start, int end, Node* parent, char code) {
    if (code == '0') {
        parent->left = new Node(parent->key + code); // ��������� ��� �������
        parent = parent->left; // ���������� �����
    }
    else if (code == '1') {
        parent->right = new Node(parent->key + code); // ��������� ��� �������
        parent = parent->right; // ���������� ������
    }
    if (start == end) { // ����� �� ����� ������
        alphabet_code[alphabet[start].first] = parent->key; // �������� ������� ����� - ����
        alphabet_decode.insert(make_pair(parent->key, alphabet[start].first));
        return;
    }

    int size = 0;
    for (int i = start; i < end; i++) {
        size += alphabet[i].second;
    }
    // ���� ��������
    int left_size = 0;
    int left_it = start;
    int middle = left_it;
    while (left_size + alphabet[left_it].second < (double)size * 0.5 && left_it < end) {
        left_size += alphabet[left_it].second;
        left_it++;
        middle++;
    }
    make_tree(start, middle, parent, '0'); // ���������� ���� �� ����� �����
    make_tree(middle + 1, end, parent, '1'); // ���������� ���� �� ������ �����
}


void ShannonFanoDecode(string code, string s, Node* current) {
    // exit criteria
    if (s.length() == 0)
    {
        cout << alphabet_decode[code] << endl;
        return;
    }
        // if we have 0 and left node
    else if (s[0] == '0' && current->left)
        ShannonFanoDecode(code + "0", s.substr(1), current->left);
        // if we have 1 and right node
    else if (s[0] == '1' && current->right)
        ShannonFanoDecode(code + "1", s.substr(1), current->right);
        // if we dont have nodes
    else if (s[0] == '0' || s[0] == '1')
    {
        cout << alphabet_decode[code];
        ShannonFanoDecode("", s, code_tree);
    }
    else ShannonFanoDecode(code, s.substr(1), current);
}

int main() {
    setlocale(LC_ALL, "Russian");
    string s = "One, two, Freddy's coming for you Three, four, better lock your door Five, six, grab a crucifix Seven, eight, gonna stay up late.";
    //string s = "Pellentesque congue lacinia magna quis vehicula. Nam at rutrum est. Nulla elementum fringilla dolor. In sed accumsan metus. Sed sit amet pretium est. Phasellus interdum sodales ex ut rhoncus. Pellentesque consequat ut libero a suscipit. Proin vitae volutpat orci, id tempor mi. Mauris tincidunt arcu ut tellus ornare finibus. Aenean est lacus, commodo a sem non, maximus rutrum dolor. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Mauris viverra ipsum nulla, eu pulvinar sapien fringilla congue.";

    alphabet = make_alphabet_tabel(s);
    cout << "��������� �������:\n"; // ������� �������
    for (int i = 0; i < alphabet.size(); i++)
        cout << "\"" << alphabet[i].first << "\" - " << alphabet[i].second << endl;

    make_tree(0, alphabet.size() - 1, code_tree, NULL); // ��������� ������

    string res_code = ""; // ��������� �����������
    cout << "���, ���������� ���������� �������-����:\n";
    for (int i = 0; i < s.length(); i++) {
        res_code += alphabet_code[s[i]];
    }

    cout << res_code;
    cout << endl;
    cout << "���������������� �����: " << s.length() * 8 << " ���" << endl;
    cout << "�������������� �����: " << res_code.length() << " ���" << endl;
    cout << "����������� ������: " << (double)res_code.length() / (s.length() * 8) << endl;
    cout << "���-� �������������: ";
    ShannonFanoDecode("", res_code, code_tree);
    return 0;
}
