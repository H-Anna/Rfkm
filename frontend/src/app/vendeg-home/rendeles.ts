export class Rendeles {
    VendegId: number;
    FizetesiMod: string;
    SzallitasiMod: string;
    EtelIdk: number[] = [];
    Etelek: {EtelID: number, Mennyi: number}[] = [];
    constructor(vendegId: number){
        this.VendegId = vendegId;
    }
    addEtel(etelId){
        if(this.vanIlyen(etelId)){
            //adott id-jú étel mennyiségének növelése eggyel
            this.Etelek.forEach(item => {
                if(item.EtelID==etelId){
                    item.Mennyi++;
                }
            });
        }
        else{
            this.Etelek.push({"EtelID":etelId, "Mennyi": 1});
            this.EtelIdk.push(etelId);
        }        
    }
    subtractEtel(etelId){
        //adott id-jú étel mennyiségének csökkentése eggyel
        if(this.vanIlyen(etelId)){
            this.Etelek.forEach(item => {
                if(item.EtelID==etelId){
                    item.Mennyi--;
                    console.log(item);
                    if(item.Mennyi==0){
                        let idx = this.EtelIdk.indexOf(etelId);
                        this.EtelIdk.splice(idx,1);
                        this.Etelek.forEach(etel => {
                            if(etel.EtelID == etelId){
                                let idx2 = this.Etelek.indexOf({"EtelID":etelId,"Mennyi":etel.Mennyi});
                                this.Etelek.splice(idx2,1);
                            }
                        });
                    }
                }
            });
        }
    }
    getEtelById(id:number){
        let idx = this.EtelIdk.indexOf(id);
        if(idx > -1){
            let etelToReturn: any;
            this.Etelek.forEach(etel => {
                if(etel.EtelID==id){
                    etelToReturn = {"EtelId":etel.EtelID,"Mennyi":etel.Mennyi};
                }
            });
            return etelToReturn;
        }
        return 0;
    }
    vanIlyen(id: number){
        if(this.EtelIdk.indexOf(id) > -1){
            return true;
        }
        return false;
    }
}


//ebből lesz létrehozva egy példány minden rendelésnél
//de ugye a komponens betöltődésekor jön létre egy új példány, tehát ha a kosárból visszanavigál erre az oldalra,
//elvesznek hogy miket adott hozzá, ez még nem tudom hogy legyen

//például:
/*{
    "VendegID": 2,
    "FizetesiMod": "utánvét",
    "SzallitasiMod": "kiszállitás",
    "Etelek": [
        {
            "EtelID": 2,
            "Mennyi": 1
        },
        {
            "EtelID": 3,
            "Mennyi": 2
        }
    ]
}
*/