int selfrec(int a){
    a = a + 2;;
    if (a < 5){
        selfrec(a);
    }
    return a;
}