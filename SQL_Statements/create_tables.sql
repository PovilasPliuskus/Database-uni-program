CREATE TABLE popl8979.Parduotuve (
    ParduotuvesID BIGSERIAL NOT NULL,
    Adresas VARCHAR(32) NOT NULL,
    Vadovas VARCHAR(32) DEFAULT 'Laisva pozicija' NOT NULL,

    PRIMARY KEY (ParduotuvesID)
);

CREATE TABLE popl8979.Produktas (
    ProduktoID BIGSERIAL NOT NULL,
    Pavadinimas VARCHAR(32) NOT NULL CONSTRAINT checkValidPavadinimas
                                     CHECK (Pavadinimas ~ '^[^0-9]+$'),
    Kaina DECIMAL(10, 2) NOT NULL CONSTRAINT checkValidPrice
                                  CHECK (Kaina > 0 AND Kaina < 500),

    PRIMARY KEY (ProduktoID)
);

CREATE TABLE popl8979.Parduoda (
    Parduotuve BIGSERIAL NOT NULL,
    Produktas BIGSERIAL NOT NULL,
    Kiekis INT DEFAULT 0 NOT NULL,

    PRIMARY KEY (Parduotuve, Produktas),
    FOREIGN KEY (Parduotuve) REFERENCES popl8979.Parduotuve(ParduotuvesID) ON DELETE CASCADE
    ON UPDATE CASCADE,
    FOREIGN KEY (Produktas) REFERENCES popl8979.Produktas(ProduktoID) ON DELETE CASCADE
    ON UPDATE CASCADE
);

CREATE VIEW popl8979.ParduotuvesIrProduktai AS
SELECT p.Adresas, pr.Pavadinimas, pr.Kaina, pd.Kiekis
FROM popl8979.Parduotuve p
JOIN popl8979.Parduoda pd ON p.parduotuvesid = pd.Parduotuve
JOIN popl8979.Produktas pr ON pr.produktoid = pd.Produktas;