# Лабораторная работа №2: Система проката

Проект представляет собой комплексную систему управления прокатом различного оборудования (строительная техника, электроника, инструменты, транспорт). Система включает в себя модули для управления клиентами, инвентарем, бронированием, платежами, техническим обслуживанием и аналитикой.

### Business Logic & Core
1. **AnalyticsEngine** 3 6 -> 3
2. **ComplianceManager** 5 8 -> 2
3. **CustomerFeedbackSystem** 4 7 -> 1
4. **CustomerManager** 6 10 -> 3
5. **DamageAssessor** 4 9 -> 1
6. **DiscountCalculator** 5 9 -> 1
7. **EmployeeManager** 4 5 -> 2
8. **EnvironmentalCompliance** 6 8 -> 2
9. **InsuranceCalculator** 5 7 -> 1
10. **InventoryOptimizer** 5 7 -> 2
11. **LateFeeCalculator** 5 7 -> 1
12. **LocationManager** 4 5 -> 2
13. **MaintenanceScheduler** 6 11 -> 2
14. **NotificationSystem** 2 6 -> 2
15. **PaymentProcessor** 7 8 -> 2
16. **PricingStrategy** 6 8 -> 2
17. **QualityAssurance** 5 8 -> 2
18. **RecommendationEngine** 6 9 -> 2
19. **ReportGenerator** 3 6 -> 3
20. **ReservationSystem** 3 7 -> 0
21. **SecuritySystem** 6 9 -> 2
22. **SupplierManager** 3 5 -> 1
23. **TrainingCoordinator** 5 9 -> 1
24. **TransportationManager** 5 7 -> 2
25. **WarrantyManager** 5 7 -> 2
26. **Customer** 17 12 -> 2
27. **Inventory** 8 10 -> 3
28. **RentalAgreement** 14 8 -> 2
29. **RentalHistory** 4 5 -> 0
30. **RentalItem** 9 8 -> 0
31. **RentalPricing** 3 6 -> 0
32. **RentalSystem** 9 8 -> 6

### Item Implementations
33. **ConstructionEquipment** 7 6 -> 0
34. **Electronics** 7 6 -> 0
35. **PartyEquipment** 7 6 -> 0
36. **Tools** 7 6 -> 0
37. **Vehicle** 8 6 -> 0

### Utilities (Static)
38. **DateUtils** 0 10 -> 0
39. **ValidationUtils** 0 12 -> 0

### Exceptions
40. **AgeRestrictionException** 0 0 -> 0
41. **CustomerBlacklistedException** 0 0 -> 0
42. **EquipmentDamageException** 0 0 -> 0
43. **InsuranceRequirementException** 0 0 -> 0
44. **InventoryMismatchException** 0 0 -> 0
45. **LicenseValidationException** 0 0 -> 0
46. **MaintenanceOverdueException** 0 0 -> 0
47. **PaymentProcessingException** 0 0 -> 0
48. **PricingCalculationException** 0 0 -> 0
49. **RentalItemUnavailableException** 0 0 -> 0
50. **ReservationConflictException** 0 0 -> 0
51. **SeasonalDemandException** 0 0 -> 0

---

## Итоговая статистика

Количество классов 51

Количество полей 214

Количество уникальных методов 286

Количество ассоциаций 54
