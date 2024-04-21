CREATE TABLE scans(
    serial varchar(100),
    scan_1 BLOB,
    scan_2 BLOB,
    scan_3 BLOB,
    scan_4 BLOB,
    scan_5 BLOB,
    FOREIGN KEY(serial) REFERENCES produto(serial)
);