INSERT INTO Branch
VALUES
    ('B001','Main Branch','Addis Ababa','0911000001'),
    ('B002','Bole Branch','Addis Ababa','0911000002'),
    ('B003','Hara Branch','Adama','0911000003');
INSERT INTO Customer
VALUES
    ('C001','Abebe Bekele','M','0912000001','Addis Ababa','2026-01-05','B001'),

    ('C002','Hana Tesfaye','F','0912000002','Addis Ababa','2026-01-06','B002'),

    ('C003','Meron Alemu','F','0912000003','Adama','2026-01-08','B003'),

    ('C004','Dawit Mekonnen','M','0912000004','Addis Ababa','2026-01-10','B001'),

    ('C005','Selamawit Guta','F','0912000005','Adama','2026-01-12','B003'),

    ('C006','Kibrom Tadesse','M','0912000006','Addis Ababa','2026-01-13','B002'),

    ('C007','Eyerusalem Haile','F','0912000007','Adama','2026-01-15','B003'),

    ('C008','Samuel Assefa','M','0912000008','Addis Ababa','2026-01-18','B001'),

    ('C009','Rahel Solomon','F','0912000009','Addis Ababa','2026-01-19','B002'),

    ('C010','Nati Yared','M','0912000010','Adama','2026-01-20','B003');
INSERT INTO Account
VALUES
    ('A1001','C001','B001','Savings',5000,'Abebe123'),

    ('A1002','C002','B002','Current',12000,'Hana123'),

    ('A1003','C003','B003','Savings',8000,'Meron123'),

    ('A1004','C004','B001','Savings',3000,'Dawit123'),

    ('A1005','C005','B003','Current',15000,'Selamawit123'),

    ('A1006','C006','B002','Savings',7000 ,'Kibrom123'),

    ('A1007','C007','B003','Savings',4500 ,'Eyerusalem123'),

    ('A1008','C008','B001','Current',20000,'Samuel123'),

    ('A1009','C009','B002','Savings',6000 ,'Rahel123'),

    ('A1010','C010','B003','Current',10000 ,'Nati123');
INSERT INTO Loan (LoanID, CustomerID, LoanType, Amount, ApplicationDate, BranchID)
VALUES
    ('L001', 'C001', 'Home Loan', 500000, '2026-02-01', 'B001'),
    ('L002', 'C003', 'Business Loan', 300000, '2026-02-04', 'B003'),
    ('L003', 'C005', 'Education Loan', 120000, '2026-02-06', 'B003'),
    ('L004', 'C007', 'Car Loan', 250000, '2026-02-08', 'B003'),
    ('L005', 'C009', 'Personal Loan', 80000, '2026-02-10', 'B002');
INSERT INTO Transactions
(TransactionID, AccountNo, BranchID, TransactionType, Amount, TransactionDate)
VALUES
    ('T001', 'A1001', 'B001', 'Deposit', 1000, '2026-03-01'),
    ('T002', 'A1002', 'B002', 'Withdrawal', 500, '2026-03-02'),
    ('T003', 'A1003', 'B003', 'Deposit', 2000, '2026-03-03'),
    ('T004', 'A1004', 'B001', 'Withdrawal', 700, '2026-03-04'),
    ('T005', 'A1005', 'B003', 'Deposit', 1500, '2026-03-05'),
    ('T006', 'A1008', 'B001', 'Deposit', 2500, '2026-03-06');
INSERT INTO Employee
(EmployeeID, FullName, Position, Email, Phone, Salary, BranchID)
VALUES
    ('E001', 'Tsegaye Endale', 'Manager', 'tsegaye@gcb.com', '0913000001', 25000, 'B001'),

    ('E002', 'Helen Fikadu', 'Cashier', 'helen@gcb.com', '0913000002', 12000, 'B001'),

    ('E003', 'Yonatan Abay', 'Accountant', 'yonatan@gcb.com', '0913000003', 15000, 'B002'),

    ('E004', 'Mulugeta Birhanu', 'Teller', 'mulugeta@gcb.com', '0913000004', 11000, 'B002'),

    ('E005', 'Eden Shiferaw', 'Customer Service', 'eden@gcb.com', '0913000005', 10000, 'B003'),

    ('E006', 'Solomon Wondimu', 'Manager', 'solomon@gcb.com', '0913000006', 25000, 'B003');


