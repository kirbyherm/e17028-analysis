

std::map<std::string, TCutG*> cuts(){


  std::map<std::string, TCutG*> cutDict;


    TCutG *ar49 = new TCutG("ar49",4);
    ar49->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    ar49->SetVarY("E_PIN2_raw");
    ar49->SetPoint(0,15500,7400);
    ar49->SetPoint(1,15000,7800);
    ar49->SetPoint(2,15500,8250);
    ar49->SetPoint(3,16000,7800);
    cutDict["ar49"] = ar49;

    TCutG *ar50 = new TCutG("ar50",4);
    ar50->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    ar50->SetVarY("E_PIN2_raw");
    ar50->SetPoint(0,14300,7600);
    ar50->SetPoint(1,13900,8000);
    ar50->SetPoint(2,14300,8400);
    ar50->SetPoint(3,14700,8000);
    cutDict["ar50"] = ar50;

    TCutG *ar51 = new TCutG("ar51",4);
    ar51->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    ar51->SetVarY("E_PIN2_raw");
    ar51->SetPoint(0,13200,8000);
    ar51->SetPoint(1,12800,8400);
    ar51->SetPoint(2,13200,8800);
    ar51->SetPoint(3,13600,8400);
    cutDict["ar51"] = ar51;



    TCutG *k51 = new TCutG("k51",4);
    k51->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    k51->SetVarY("E_PIN2_raw");
    k51->SetPoint(0,16200,8300);
    k51->SetPoint(1,15600,8700);
    k51->SetPoint(2,16200,9100);
    k51->SetPoint(3,16700,8700);
    cutDict["k51"] = k51;

    TCutG *k52 = new TCutG("k52",4);
    k52->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    k52->SetVarY("E_PIN2_raw");
    k52->SetPoint(0,15100,8500);
    k52->SetPoint(1,14600,8900);
    k52->SetPoint(2,15100,9300);
    k52->SetPoint(3,15500,8900);
    cutDict["k52"] = k52;

    TCutG *k53 = new TCutG("k53",4);
    k53->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    k53->SetVarY("E_PIN2_raw");
    k53->SetPoint(0,14100,8800);
    k53->SetPoint(1,13600,9200);
    k53->SetPoint(2,14100,9600);
    k53->SetPoint(3,14500,9200);
    cutDict["k53"] = k53;



    TCutG *ca52 = new TCutG("ca52",4);
    ca52->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    ca52->SetVarY("E_PIN2_raw");
    ca52->SetPoint(0,17800,8800);
    ca52->SetPoint(1,17300,9200);
    ca52->SetPoint(2,17800,9500);
    ca52->SetPoint(3,18300,9200);
    cutDict["ca52"] = ca52;

    TCutG *ca53 = new TCutG("ca53",4);
    ca53->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    ca53->SetVarY("E_PIN2_raw");
    ca53->SetPoint(0,16800,9100);
    ca53->SetPoint(1,16300,9400);
    ca53->SetPoint(2,16800,9850);
    ca53->SetPoint(3,17300,9400);
    cutDict["ca53"] = ca53;

    TCutG *ca54 = new TCutG("ca54",4);
    ca54->SetVarX(("(energy_TAC_I2N_I2S*0.8+energy_TAC_PIN2_I2S+1500)"));
    ca54->SetVarY("PIN2");
    //center at 16200, 9500
    ca54->SetPoint(0,16200,9000);
    ca54->SetPoint(1,15700,9500);
    ca54->SetPoint(2,16200,10000);
    ca54->SetPoint(3,16700,9500);
    cutDict["ca54"] = ca54;

    TCutG *ca55 = new TCutG("ca55",4);
    ca55->SetVarX(("(energy_TAC_I2N_I2S*0.8+energy_TAC_PIN2_I2S+1500)"));
    ca55->SetVarY("PIN2");
    //center at 15200, 9800
    ca55->SetPoint(0,15200,9300);
    ca55->SetPoint(1,14700,9800);
    ca55->SetPoint(2,15200,10300);
    ca55->SetPoint(3,15700,9800);
    cutDict["ca55"] = ca55;

    TCutG *sc55 = new TCutG("sc55",4);
    sc55->SetVarX(("(E_TAC_I2S_I2N_raw*0.8+E_TAC_PIN2_I2S_raw+1500)"));
    sc55->SetVarY("E_PIN2_raw");
    //center at 17600, 10000
    sc55->SetPoint(0,17600,9500);
    sc55->SetPoint(1,17100,10000);
    sc55->SetPoint(2,17600,10500);
    sc55->SetPoint(3,18100,10000);
    cutDict["sc55"] = sc55;
  
    TCutG *sc56 = new TCutG("sc56",4);
    sc56->SetVarX(("(E_TAC_I2S_I2N_raw*0.8+E_TAC_PIN2_I2S_raw+1500)"));
    sc56->SetVarY("E_PIN2_raw");
    //center at 16700, 10400
    sc56->SetPoint(0,16700,9900);
    sc56->SetPoint(1,16200,10400);
    sc56->SetPoint(2,16700,10900);
    sc56->SetPoint(3,17200,10400);
    cutDict["sc56"] = sc56;
  
    TCutG *sc57 = new TCutG("sc57",4);
    sc57->SetVarX(("(E_TAC_I2S_I2N_raw*0.8+E_TAC_PIN2_I2S_raw+1500)"));
    sc57->SetVarY("E_PIN2_raw");
    //center at 15800, 10800
    sc57->SetPoint(0,15800,10300);
    sc57->SetPoint(1,15300,10800);
    sc57->SetPoint(2,15800,11300);
    sc57->SetPoint(3,16300,10800);
    cutDict["sc57"] = sc57;
  
    TCutG *sc58 = new TCutG("sc58",4);
    sc58->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    sc58->SetVarY("E_PIN2_raw");
    //center at 14700, 11100
    sc58->SetPoint(0,14800,10600);
    sc58->SetPoint(1,14300,11100);
    sc58->SetPoint(2,14800,11600);
    sc58->SetPoint(3,15300,11100);
    cutDict["sc58"] = sc58;

  
  
    TCutG *ti57 = new TCutG("ti57",4);
    ti57->SetVarX(("(E_TAC_I2S_I2N_raw*0.8+E_TAC_PIN2_I2S_raw+1500)"));
    ti57->SetVarY("E_PIN2_raw");
    // 18200, 11000
    ti57->SetPoint(0,18200,10500);
    ti57->SetPoint(1,17700,11000);
    ti57->SetPoint(2,18200,11500);
    ti57->SetPoint(3,18700,11000);
    cutDict["ti57"] = ti57;
  
    TCutG *ti58 = new TCutG("ti58",4);
    ti58->SetVarX(("(E_TAC_I2S_I2N_raw*0.8+E_TAC_PIN2_I2S_raw+1500)"));
    ti58->SetVarY("E_PIN2_raw");
    // 17300, 11300
    ti58->SetPoint(0,17300,10800);
    ti58->SetPoint(1,16800,11300);
    ti58->SetPoint(2,17300,11800);
    ti58->SetPoint(3,17800,11300);
    cutDict["ti58"] = ti58;
  
    TCutG *ti59 = new TCutG("ti59",4);
    ti59->SetVarX(("(E_TAC_I2S_I2N_raw*0.8+E_TAC_PIN2_I2S_raw+1500)"));
    ti59->SetVarY("E_PIN2_raw");
    // 16400, 11700
    ti59->SetPoint(0,16400,11200);
    ti59->SetPoint(1,15900,11700);
    ti59->SetPoint(2,16400,12200);
    ti59->SetPoint(3,16900,11700);
    cutDict["ti59"] = ti59;
  
    TCutG *ti60 = new TCutG("ti60",4);
    ti60->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    ti60->SetVarY("E_PIN2_raw");
    // 15400, 12000
    ti60->SetPoint(0,15400,11500);
    ti60->SetPoint(1,14900,12000);
    ti60->SetPoint(2,15400,12500);
    ti60->SetPoint(3,15900,12000);
    cutDict["ti60"] = ti60;

  
  
    TCutG *v59 = new TCutG("v59",4);
    v59->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    v59->SetVarY("E_PIN2_raw");
    // 18500, 12000
    v59->SetPoint(0,18500,11500);
    v59->SetPoint(1,18000,12000);
    v59->SetPoint(2,18500,12500);
    v59->SetPoint(3,19000,12000);
    cutDict["v59"] = v59;
  
    TCutG *v60 = new TCutG("v60",4);
    v60->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    v60->SetVarY("E_PIN2_raw");
    // 17700, 12300
    v60->SetPoint(0,17700,11800);
    v60->SetPoint(1,17200,12300);
    v60->SetPoint(2,17700,12800);
    v60->SetPoint(3,18200,12300);
    cutDict["v60"] = v60;
  
    TCutG *v61 = new TCutG("v61",4);
    v61->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    v61->SetVarY("E_PIN2_raw");
    // 16800, 12700
    v61->SetPoint(0,16800,12200);
    v61->SetPoint(1,16300,12700);
    v61->SetPoint(2,16800,13200);
    v61->SetPoint(3,17300,12700);
    cutDict["v61"] = v61;
  
    TCutG *v62 = new TCutG("v62",4);
    v62->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    v62->SetVarY("E_PIN2_raw");
    // 16200, 12700
    v62->SetPoint(0,16200,12200);
    v62->SetPoint(1,15700,12700);
    v62->SetPoint(2,16200,13200);
    v62->SetPoint(3,16700,12700);
    cutDict["v62"] = v62;

  
  
    TCutG *cr61 = new TCutG("cr61",4);
    cr61->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    cr61->SetVarY("E_PIN2_raw");
    cr61->SetPoint(0,18950,12550);
    cr61->SetPoint(1,18450,12950);
    cr61->SetPoint(2,18950,13300);
    cr61->SetPoint(3,19450,12950);
    cutDict["cr61"] = cr61;
  
    TCutG *cr62 = new TCutG("cr62",4);
    cr62->SetVarX(("(E_TAC_I2S_I2N_raw*0.8+E_TAC_PIN2_I2S_raw+1500)"));
    cr62->SetVarY("E_PIN2_raw");
    // 18150, 13400
    cr62->SetPoint(0,18150,12900);
    cr62->SetPoint(1,17650,13400);
    cr62->SetPoint(2,18150,13900);
    cr62->SetPoint(3,18650,13400);
    cutDict["cr62"] = cr62;
  
    TCutG *cr63 = new TCutG("cr63",4);
    cr63->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    cr63->SetVarY("E_PIN2_raw");
    // 17400, 13700
    cr63->SetPoint(0,17400,13200);
    cr63->SetPoint(1,16900,13700);
    cr63->SetPoint(2,17400,14200);
    cr63->SetPoint(3,17900,13700);
    cutDict["cr63"] = cr63;
  
    TCutG *cr64 = new TCutG("cr64",4);
    cr64->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    cr64->SetVarY("E_PIN2_raw");
    cr64->SetPoint(0,16500,13400);
    cr64->SetPoint(1,16200,13800);
    cr64->SetPoint(2,16500,14200);
    cr64->SetPoint(3,16800,13800);
    cutDict["cr64"] = cr64;

  
  
    TCutG *mn63 = new TCutG("mn63",4);
    mn63->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    mn63->SetVarY("E_PIN2_raw");
    mn63->SetPoint(0,19350,13750);
    mn63->SetPoint(1,19000,14100);
    mn63->SetPoint(2,19350,14500);
    mn63->SetPoint(3,19750,13750);
    cutDict["mn63"] = mn63;

    TCutG *mn64 = new TCutG("mn64",4);
    mn64->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    mn64->SetVarY("E_PIN2_raw");
    mn64->SetPoint(0,18600,14000);
    mn64->SetPoint(1,18100,14500);
    mn64->SetPoint(2,18600,15000);
    mn64->SetPoint(3,19100,14500);
    cutDict["mn64"] = mn64;

    TCutG *mn65 = new TCutG("mn65",4);
    mn65->SetVarX(("(E_TAC_I2S_I2N_raw*1.0+E_TAC_PIN2_I2S_raw+1500)"));
    mn65->SetVarY("E_PIN2_raw");
    mn65->SetPoint(0,17800,14300);
    mn65->SetPoint(1,17300,14800);
    mn65->SetPoint(2,17800,15300);
    mn65->SetPoint(3,18300,14800);
    cutDict["mn65"] = mn65;

  
  
  
  
  
  
    TCutG *pin12 = new TCutG("pin12",5);
    pin12->SetVarX(("E_PIN2_raw"));
    pin12->SetVarY("E_PIN1_raw");
    pin12->SetPoint(0,1,300);
    pin12->SetPoint(1,17000,9400);
    pin12->SetPoint(2,20000,9400);
    pin12->SetPoint(3,500,1);
    pin12->SetPoint(4,1,1);
    cutDict["pin12"] = pin12;
  
    TCutG *i2pos = new TCutG("i2pos",6);
    i2pos->SetVarY(("E_PIN2_raw"));
    i2pos->SetVarX(("E_TAC_I2S_I2N"));
    i2pos->SetPoint(0,3000,8000);
    i2pos->SetPoint(1,3600,18000);
    i2pos->SetPoint(2,5600,18000);
    i2pos->SetPoint(3,4600,7000);
    i2pos->SetPoint(4,4800,5000);
    i2pos->SetPoint(5,3000,5000);
    cutDict["i2pos"] = i2pos;

  return cutDict;
}
std::map<std::string, double> qvalDict(){


  std::map<std::string, double> qDict;


    qDict["ar49"] = 12400;
    qDict["ar50"] = 12400;
    qDict["ar51"] = 15800;

    qDict["k51"] = 13816;
    qDict["k52"] = 17130;
    qDict["k53"] = 17090;

    qDict["ca52"] = 6180;
    qDict["ca53"] = 9520;
    qDict["ca54"] = 8700;
    qDict["ca55"] = 11800;

    qDict["sc55"] = 11500;
    qDict["sc56"] = 14500;
    qDict["sc57"] = 12860;
    qDict["sc58"] = 16200;

    qDict["ti57"] = 10500;
    qDict["ti58"] = 9290;
    qDict["ti59"] = 12300;
    qDict["ti60"] = 10900;

    qDict["v59"] = 10300;
    qDict["v60"] = 13400;
    qDict["v61"] = 12000;
    qDict["v62"] = 15400;

    qDict["cr61"] = 9270;
    qDict["cr62"] = 7630;
    qDict["cr63"] = 10900;
    qDict["cr64"] = 9500;

    qDict["mn63"] = 8749;
    qDict["mn64"] = 11981;
    qDict["mn65"] = 10251;

  return qDict;
}
