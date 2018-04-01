int getSum(int a, int b) {
    
    if (0 == b) return a; //当没有进位时
    int sum = a^b;
    int carry = (a & b) << 1; //进位
    return getSum(sum, carry);
}