from django.contrib import admin

from .models import SensorData

@admin.register(SensorData)
class SensorDataAdmin(admin.ModelAdmin):
    list_display=('temperature','humidity','timestamp')

