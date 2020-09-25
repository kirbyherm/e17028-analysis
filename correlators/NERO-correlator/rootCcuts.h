#include "TGraph.h"


void cuts(){
    TCutG *b1 = new TCutG("PIN2TOFb1",4);
    b1->SetVarX("energy_TAC_PIN2_I2N");
    b1->SetVarY("PIN1+PIN2");
    b1->SetPoint(0,5000,14000);
    b1->SetPoint(1,10000,12000);
    b1->SetPoint(2,9000,11000);
    b1->SetPoint(3,5000,12500);
    TCutG *b2 = new TCutG("PIN2TOFb2",4);
    b2->SetVarX("energy_TAC_PIN2_I2N");
    b2->SetVarY("PIN1+PIN2");
    b2->SetPoint(0,5500,14800);
    b2->SetPoint(1,9000,13300);
    b2->SetPoint(2,9000,13000);
    b2->SetPoint(3,5500,14250);
    TCutG *b3 = new TCutG("PIN2TOFb3",4);
    b3->SetVarX("energy_TAC_PIN2_I2N");
    b3->SetVarY("PIN1+PIN2");
    b3->SetPoint(0,6000,16250);
    b3->SetPoint(1,11500,14000);
    b3->SetPoint(2,11000,13500);
    b3->SetPoint(3,6000,15500);
    TCutG *b4 = new TCutG("PIN2TOFb4",4);
    b4->SetVarX("energy_TAC_PIN2_I2N");
    b4->SetVarY("PIN1+PIN2");
    b4->SetPoint(0,6250,17800);
    b4->SetPoint(1,11500,15250);
    b4->SetPoint(2,11000,14750);
    b4->SetPoint(3,6000,17300);
    TCutG *b5 = new TCutG("PIN2TOFb5",4);
    b5->SetVarX("energy_TAC_PIN2_I2N");
    b5->SetVarY("PIN1+PIN2");
    b5->SetPoint(0,7000,19300);
    b5->SetPoint(1,11500,16750);
    b5->SetPoint(2,11500,16000);
    b5->SetPoint(3,7000,18450);
    TCutG *b6 = new TCutG("PIN2TOFb6",4);
    b6->SetVarX("energy_TAC_PIN2_I2N");
    b6->SetVarY("PIN1+PIN2");
    b6->SetPoint(0,7300,20700);
    b6->SetPoint(1,12000,18000);
    b6->SetPoint(2,11750,17500);
    b6->SetPoint(3,7500,20000);
    TCutG *b7 = new TCutG("PIN2TOFb7",4);
    b7->SetVarX("energy_TAC_PIN2_I2N");
    b7->SetVarY("PIN1+PIN2");
    b7->SetPoint(0,8000,22000);
    b7->SetPoint(1,11750,20000);
    b7->SetPoint(2,11500,19500);
    b7->SetPoint(3,8000,21500);
    TCutG *b8 = new TCutG("PIN2TOFb8",4);
    b8->SetVarX("energy_TAC_PIN2_I2N");
    b8->SetVarY("PIN1+PIN2");
    b8->SetPoint(0,8500,24000);
    b8->SetPoint(1,11500,22000);
    b8->SetPoint(2,11250,21250);
    b8->SetPoint(3,8500,23000);
//    ti58->SetPoint(0,17250,11650);
//    ti58->SetPoint(1,17400,11500);
//    ti58->SetPoint(2,17250,11250);
//    ti58->SetPoint(3,17250,11200);
//    ti58->SetPoint(4,16900,11200);
//    ti58->SetPoint(5,16800,11500);
//    ti58->SetPoint(6,17000,11650);
    TCutG *pin12 = new TCutG("pin12",5);
    pin12->SetVarX(("PIN2"));
    pin12->SetVarY("PIN1");
    pin12->SetPoint(0,1,(int) (300 * 1.06637654974));
    pin12->SetPoint(1,19000,(int) (10600 * 1.06637654974));
    pin12->SetPoint(2,19000,(int) (8800 * 1.06637654974));
    pin12->SetPoint(3,500,1);
    pin12->SetPoint(4,1,1);
    TCutG *i2pos = new TCutG("i2pos",5);
    i2pos->SetVarY(("PIN2"));
    i2pos->SetVarX(("energy_TAC_I2N_I2S"));
    i2pos->SetPoint(0,3700,9000);
    i2pos->SetPoint(1,4300,18000);
    i2pos->SetPoint(2,4900,18000);
    i2pos->SetPoint(3,4800,14000);
    i2pos->SetPoint(4,4300,9000);
//    i2pos->SetPoint(0,-1,-1);
//    i2pos->SetPoint(1,-1,132000);
//    i2pos->SetPoint(2,100,132000);
//    i2pos->SetPoint(3,100,-1);


    TCutG *veto = new TCutG("veto",4);
    veto->SetVarY(("energy_Veto"));
    veto->SetVarX(("energy_Veto"));
    veto->SetPoint(0,-1,-1);
    veto->SetPoint(1,-1,1);
    veto->SetPoint(2,1,1);
    veto->SetPoint(3,1,-1);



    TCutG *ar49 = new TCutG("ar49",4);
    ar49->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ar49->SetVarY("PIN2");
    ar49->SetPoint(0,15500,7400);
    ar49->SetPoint(1,15000,7800);
    ar49->SetPoint(2,15500,8250);
    ar49->SetPoint(3,16000,7800);

    TCutG *ar50 = new TCutG("ar50",4);
    ar50->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ar50->SetVarY("PIN2");
    ar50->SetPoint(0,14300,7600);
    ar50->SetPoint(1,13900,8000);
    ar50->SetPoint(2,14300,8400);
    ar50->SetPoint(3,14700,8000);

    TCutG *ar51 = new TCutG("ar51",4);
    ar51->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ar51->SetVarY("PIN2");
    ar51->SetPoint(0,13200,8000);
    ar51->SetPoint(1,12800,8400);
    ar51->SetPoint(2,13200,8800);
    ar51->SetPoint(3,13600,8400);



    TCutG *k51 = new TCutG("k51",4);
    k51->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    k51->SetVarY("PIN2");
    k51->SetPoint(0,16200,8300);
    k51->SetPoint(1,15600,8700);
    k51->SetPoint(2,16200,9100);
    k51->SetPoint(3,16700,8700);

    TCutG *k52 = new TCutG("k52",4);
    k52->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    k52->SetVarY("PIN2");
    k52->SetPoint(0,15100,8500);
    k52->SetPoint(1,14600,8900);
    k52->SetPoint(2,15100,9300);
    k52->SetPoint(3,15500,8900);

    TCutG *k53 = new TCutG("k53",4);
    k53->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    k53->SetVarY("PIN2");
    k53->SetPoint(0,14100,8800);
    k53->SetPoint(1,13600,9200);
    k53->SetPoint(2,14100,9600);
    k53->SetPoint(3,14500,9200);



    TCutG *ca52 = new TCutG("ca52",4);
    ca52->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ca52->SetVarY("PIN2");
    ca52->SetPoint(0,17800,8800);
    ca52->SetPoint(1,17300,9200);
    ca52->SetPoint(2,17800,9500);
    ca52->SetPoint(3,18300,9200);

    TCutG *ca53 = new TCutG("ca53",4);
    ca53->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ca53->SetVarY("PIN2");
    ca53->SetPoint(0,16800,9100);
    ca53->SetPoint(1,16300,9400);
    ca53->SetPoint(2,16800,9850);
    ca53->SetPoint(3,17300,9400);

    TCutG *ca54 = new TCutG("ca54",4);
    ca54->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ca54->SetVarY("PIN2");
    ca54->SetPoint(0,15900,9400);
    ca54->SetPoint(1,15400,9700);
    ca54->SetPoint(2,15900,10000);
    ca54->SetPoint(3,16300,9700);

    TCutG *ca55 = new TCutG("ca55",4);
    ca55->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ca55->SetVarY("PIN2");
    ca55->SetPoint(0,14800,9600);
    ca55->SetPoint(1,14300,10000);
    ca55->SetPoint(2,14800,10400);
    ca55->SetPoint(3,15300,10000);



    TCutG *sc55 = new TCutG("sc55",4);
    sc55->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    sc55->SetVarY("PIN2");
    sc55->SetPoint(0,17400,9800);
    sc55->SetPoint(1,17000,10200);
    sc55->SetPoint(2,17400,10550);
    sc55->SetPoint(3,18000,10200);

    TCutG *sc56 = new TCutG("sc56",4);
    sc56->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    sc56->SetVarY("PIN2");
    sc56->SetPoint(0,16500,10100);
    sc56->SetPoint(1,16100,10500);
    sc56->SetPoint(2,16500,10850);
    sc56->SetPoint(3,16900,10500);

    TCutG *sc57 = new TCutG("sc57",4);
    sc57->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    sc57->SetVarY("PIN2");
    sc57->SetPoint(0,15700,10400);
    sc57->SetPoint(1,15200,10800);
    sc57->SetPoint(2,15700,11400);
    sc57->SetPoint(3,16200,10800);

    TCutG *sc58 = new TCutG("sc58",4);
    sc58->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    sc58->SetVarY("PIN2");
    sc58->SetPoint(0,14700,10800);
    sc58->SetPoint(1,14100,11200);
    sc58->SetPoint(2,14700,11600);
    sc58->SetPoint(3,15200,11200);



    TCutG *ti57 = new TCutG("ti57",4);
    ti57->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ti57->SetVarY("PIN2");
    ti57->SetPoint(0,18000,10600);
    ti57->SetPoint(1,17600,11000);
    ti57->SetPoint(2,18000,11350);
    ti57->SetPoint(3,18400,11000);

    TCutG *ti58 = new TCutG("ti58",7);
    ti58->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ti58->SetVarY("PIN2");
    ti58->SetPoint(0,17250,11650);
    ti58->SetPoint(1,17400,11500);
    ti58->SetPoint(2,17450,11250);
    ti58->SetPoint(3,17450,10800);
    ti58->SetPoint(4,16900,11200);
    ti58->SetPoint(5,16800,11500);
    ti58->SetPoint(6,17000,11650);

    TCutG *ti59 = new TCutG("ti59",4);
    ti59->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ti59->SetVarY("PIN2");
    ti59->SetPoint(0,16300,11400);
    ti59->SetPoint(1,15800,11800);
    ti59->SetPoint(2,16300,12250);
    ti59->SetPoint(3,16800,11800);

    TCutG *ti60 = new TCutG("ti60",4);
    ti60->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ti60->SetVarY("PIN2");
    ti60->SetPoint(0,15300,11600);
    ti60->SetPoint(1,15000,12000);
    ti60->SetPoint(2,15300,12400);
    ti60->SetPoint(3,15700,12000);



    TCutG *v59 = new TCutG("v59",4);
    v59->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    v59->SetVarY("PIN2");
    v59->SetPoint(0,18500,11600);
    v59->SetPoint(1,18100,12000);
    v59->SetPoint(2,18500,12350);
    v59->SetPoint(3,18900,12000);

    TCutG *v60 = new TCutG("v60",4);
    v60->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    v60->SetVarY("PIN2");
    v60->SetPoint(0,17650,11900);
    v60->SetPoint(1,17300,12300);
    v60->SetPoint(2,17650,12650);
    v60->SetPoint(3,18100,12300);

    TCutG *v61 = new TCutG("v61",4);
    v61->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    v61->SetVarY("PIN2");
    v61->SetPoint(0,16800,12300);
    v61->SetPoint(1,16300,12700);
    v61->SetPoint(2,16800,13150);
    v61->SetPoint(3,17300,12700);

    TCutG *v62 = new TCutG("v62",4);
    v62->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    v62->SetVarY("PIN2");
    v62->SetPoint(0,15900,12400);
    v62->SetPoint(1,15600,12900);
    v62->SetPoint(2,15900,13400);
    v62->SetPoint(3,16300,12900);



    TCutG *cr61 = new TCutG("cr61",4);
    cr61->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    cr61->SetVarY("PIN2");
    cr61->SetPoint(0,18950,12550);
    cr61->SetPoint(1,18450,12950);
    cr61->SetPoint(2,18950,13300);
    cr61->SetPoint(3,19450,12950);

    TCutG *cr62 = new TCutG("cr62",4);
    cr62->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    cr62->SetVarY("PIN2");
    cr62->SetPoint(0,18150,12850);
    cr62->SetPoint(1,17750,13250);
    cr62->SetPoint(2,18150,13600);
    cr62->SetPoint(3,18550,13250);

    TCutG *cr63 = new TCutG("cr63",4);
    cr63->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    cr63->SetVarY("PIN2");
    cr63->SetPoint(0,17400,13200);
    cr63->SetPoint(1,16900,13600);
    cr63->SetPoint(2,17400,14000);
    cr63->SetPoint(3,17900,13600);

    TCutG *cr64 = new TCutG("cr64",4);
    cr64->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    cr64->SetVarY("PIN2");
    cr64->SetPoint(0,16500,13400);
    cr64->SetPoint(1,16200,13800);
    cr64->SetPoint(2,16500,14200);
    cr64->SetPoint(3,16800,13800);



    TCutG *mn63 = new TCutG("mn63",4);
    mn63->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    mn63->SetVarY("PIN2");
    mn63->SetPoint(0,19350,13750);
    mn63->SetPoint(1,19000,14100);
    mn63->SetPoint(2,19350,14500);
    mn63->SetPoint(3,19750,13750);

    TCutG *mn64 = new TCutG("mn64",4);
    mn64->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    mn64->SetVarY("PIN2");
    mn64->SetPoint(0,18600,14000);
    mn64->SetPoint(1,18100,14500);
    mn64->SetPoint(2,18600,15000);
    mn64->SetPoint(3,19100,14500);

    TCutG *mn65 = new TCutG("mn65",4);
    mn65->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    mn65->SetVarY("PIN2");
    mn65->SetPoint(0,17800,14300);
    mn65->SetPoint(1,17300,14800);
    mn65->SetPoint(2,17800,15300);
    mn65->SetPoint(3,18300,14800);

//    TCutG *mn65 = new TCutG("mn65",4);
//    mn65->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
//    mn65->SetVarY("PIN2");
//    mn65->SetPoint(0,16500,13400);
//    mn65->SetPoint(1,16200,13800);
//    mn65->SetPoint(2,16500,14200);
//    mn65->SetPoint(3,16800,13800);


    TCutG *ca = new TCutG("ca",4);
    ca->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ca->SetVarY("PIN2");
    ca->SetPoint(0,15000,12000);
    ca->SetPoint(1,19500,10300);
    ca->SetPoint(2,19500,9000);
    ca->SetPoint(3,15000,10500);

    TCutG *sc = new TCutG("sc",4);
    sc->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    sc->SetVarY("PIN2");
    sc->SetPoint(0,14500,13500);
    sc->SetPoint(1,20000,11300);
    sc->SetPoint(2,20000,10000);
    sc->SetPoint(3,14500,12000);

    TCutG *ti = new TCutG("ti",4);
    ti->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    ti->SetVarY("PIN2");
    ti->SetPoint(0,14500,14300);
    ti->SetPoint(1,20000,12000);
    ti->SetPoint(2,20000,11000);
    ti->SetPoint(3,14500,13500);

    TCutG *v = new TCutG("v",4);
    v->SetVarX(("(energy_TAC_I2N_I2S*1.0+energy_TAC_PIN2_I2S+1500)"));
    v->SetVarY("PIN2");
    v->SetPoint(0,15000,15500);
    v->SetPoint(1,20000,13500);
    v->SetPoint(2,20000,12500);
    v->SetPoint(3,15000,14500);

















    TCutG *elow03 = new TCutG("elow03",4);
    elow03->SetVarX(("cStripLowFront"));
    elow03->SetVarY("cStripLowBack");
    elow03->SetPoint(0,-1.001,2.99);
    elow03->SetPoint(1,15.001,2.99);
    elow03->SetPoint(2,15.001,3.001);
    elow03->SetPoint(3,-1.001,3.001);

    TCutG *elow04 = new TCutG("elow04",4);
    elow04->SetVarX(("cStripLowFront"));
    elow04->SetVarY("cStripLowBack");
    elow04->SetPoint(0,-1.001,3.99);
    elow04->SetPoint(1,15.001,3.99);
    elow04->SetPoint(2,15.001,4.001);
    elow04->SetPoint(3,-1.001,4.001);

    TCutG *elow = new TCutG("elow",4);
    elow->SetVarX(("cStripLowFront"));
    elow->SetVarY("cStripLowBack");
    elow->SetPoint(0,-0.001,-0.99);
    elow->SetPoint(1,15.001,-0.99);
    elow->SetPoint(2,15.001,15.001);
    elow->SetPoint(3,-0.001,15.001);


    return 0;
}
