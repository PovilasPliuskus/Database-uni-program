INSERT INTO popl8979.Parduotuve (Adresas, Vadovas)
VALUES
    ('Gelezinkelio g. 16', 'Jonas Jonaitis'),
    ('Ozo g. 25', 'Rimas Rimavicius');

INSERT INTO popl8979.Produktas (Pavadinimas, Kaina)
VALUES
    ('Braskes', 2.47),
    ('Pomidorai', 1.56),
    ('Suris', 1.99);

INSERT INTO popl8979.Parduoda (Parduotuve, Produktas, Kiekis)
VALUES
    (1, 1, 5),
    (1, 2, 6),
    (2, 1, 8);