class Manacher
{
    vector<int> p;
    void run_manacher(string s)
    {
        int n = s.size();
        p.assign(n, 1);

        int l = 1, r = 1;
        for (int i = 1; i < n; i++)
        {
            p[i] = max(0, min(r - i, p[l + r - 1]));
            while (i - p[i] >= 0 && i + p[i] < n && s[i - p[i]] == s[i + p[i]])
                p[i]++;
            if (i + p[i] > r)
            {
                l = i - p[i];
                r = i + p[i];
            }
        }
    }
    void build(string s)
    {
        string t = "#";
        for (char c : s)
        {
            t+=c;
            t+='#';
        }
        run_manacher(t);
        // cout<<t<<endl;
        // for(int a:p)
        //     cout<<a<<"  ";
        // cout<<endl;
    }

public:
    Manacher(string s)
    {
        build(s);
    }

    int getLongest(int c,bool odd){
        int pos=2*c+1+(!odd);
        return p[pos]-1;
    }

    bool checkPal(int l,int r){
        if((r-l+1)<=getLongest((l+r)/2,l%2==r%2))
            return true;
        return false;
    }
};
