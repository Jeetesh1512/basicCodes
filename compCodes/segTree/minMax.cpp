class SEG{
    vector<int> seg;
public:
    SEG(int n){
        seg.resize(4*n+1);
    }

    void build(int ind,int low,int high,vector<int> &nums){
        if(low==high){
            seg[ind]=nums[low];
            return;
        }

        int mid = low+((high-low)>>1);

        build(2*ind+1,low,mid,nums);
        build(2*ind+2,mid+1,high,nums);

        seg[ind] = max(seg[2*ind+1],seg[2*ind+2]);
    }

    int query(int ind,int low,int high,int l,int r){
        if(l>high || r<low)
            return INT_MIN;

        if(low>=l && high<=r)
            return seg[ind];

        int mid = low+((high-low)>>1);

        int left = query(2*ind+1,low,mid);
        int right = query(2*ind+2,mid+1,high);

        return max(left,right);
    }

    void update(int ind,int low,int high,int i,int val){
        if(low==high){
            seg[ind]=val;
            return;
        }

        int mid = low+((high-low)>>1);

        if(i<=mid)
            update(2*ind+1,low,mid,i,val);
        else
            update(2*ind+2,mid+1,high,i,val);

        seg[ind] = max(seg[2*ind+1],seg[2*ind+2]);        
    }
};
