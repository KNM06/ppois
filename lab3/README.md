# Лабораторная работа №2: Система управления недвижимостью 

Проект представляет собой масштабную систему для управления арендой недвижимости. Система охватывает полный жизненный цикл аренды: от управления объектами (квартиры и коммерческие помещения) и инвентарем до обработки договоров, финансовых транзакций, технического обслуживания, инспекций и отчетности.


### Managers & Systems
1. **AvailabilityCalendar** 10 9 -> 1
2. **BookingSystem** 7 10 -> 2
3. **CommissionCalculator** 7 10 -> 0
4. **ContractManager** 7 14 -> 3
5. **DocumentManager** 7 11 -> 3
6. **InspectionManager** 5 10 -> 1
7. **MaintenanceManager** 6 13 -> 2
8. **NotificationSystem** 7 13 -> 0
9. **OwnerManager** 5 11 -> 3
10. **PaymentManager** 6 9 -> 2
11. **PropertyManager** 5 11 -> 3
12. **RentalManager** 6 13 -> 4
13. **RentalPricing** 7 10 -> 0
14. **RentalPricingStrategy** 9 9 -> 0
15. **ReportingEngine** 6 10 -> 3
16. **TenantManager** 5 10 -> 3

### Core Entities
17. **Address** 7 4 -> 0
18. **Agency** 12 12 -> 2
19. **Agent** 12 11 -> 1
20. **Amenity** 11 9 -> 0
21. **Document** 13 11 -> 2
22. **FloorPlan** 13 9 -> 1
23. **Inventory** 12 10 -> 1
24. **Owner** 10 7 -> 1
25. **Photo** 12 9 -> 1
26. **PropertyFeature** 10 9 -> 0
27. **Property** 6 5 -> 3
28. **PropertyStatus** 9 9 -> 0
29. **PropertyType** 10 9 -> 0
30. **Tenant** 10 8 -> 1
31. **Video** 13 10 -> 1

### Property Implementations
32. **Apartment** 8 7 -> 0
33. **CommercialSpace** 9 8 -> 0
34. **House** 9 8 -> 0
35. **Land** 9 8 -> 0

### Finance
36. **Commission** 11 10 -> 2
37. **FinancialAnalysis** 5 10 -> 2
38. **FinancialReport** 13 9 -> 2
39. **Invoice** 12 9 -> 2
40. **Payment** 10 9 -> 2
41. **Receipt** 11 9 -> 1
42. **RentalContract** 10 9 -> 3
43. **SecurityDeposit** 11 9 -> 2
44. **TaxCalculator** 8 9 -> 0

### Maintenance
45. **MaintenanceCost** 11 9 -> 1
46. **MaintenanceHistory** 9 9 -> 2
47. **MaintenancePriority** 9 9 -> 0
48. **MaintenanceRequest** 12 8 -> 2
49. **MaintenanceSchedule** 11 11 -> 2
50. **MaintenanceStatus** 10 9 -> 0
51. **MaintenanceTask** 12 9 -> 1

### Exceptions
52. **CommissionCalculationException** 0 0 -> 0
53. **ContractViolationException** 0 0 -> 0
54. **DocumentExpiredException** 0 0 -> 0
55. **FinancialReportException** 0 0 -> 0
56. **InspectionFailedException** 0 0 -> 0
57. **InvalidLeaseTermException** 0 0 -> 0
58. **MaintenanceEmergencyException** 0 0 -> 0
59. **PaymentProcessingException** 0 0 -> 0
60. **PropertyNotAvailableException** 0 0 -> 0
61. **RentalException** 0 0 -> 0
62. **SecurityDepositException** 0 0 -> 0
63. **TaxCalculationException** 0 0 -> 0
64. **TenantNotQualifiedException** 0 0 -> 0

---

## 📊 Итоговая статистика

Количество классов 64

Количество полей 536

Количество уникальных методов 607

Количество ассоциаций 69
