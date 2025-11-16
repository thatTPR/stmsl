import xml.etree.ElementTree as ET
import requests
from textwrap import indent

# URL of the SPIR-V registry XML
SPIRV_XML_URL = "https://registry.khronos.org/SPIR-V/specs/unified1/SPIRV.html"

# Download and parse XML
print("Downloading spir-v.xml ...")
xml_data = requests.get(SPIRV_XML_URL)
print(xml_data) 
xml_data.raise_for_status()

root = ET.fromstring(xml_data.content)

# Function to clean text
def clean(s):
    return s.strip() if s else ""

print("\n=== SPIR-V Enumerants ===\n")

for enum in root.findall(".//enums"):
    enum_name = enum.get("name", "UnnamedEnum")
    enum_category = enum.get("category", "")
    enum_type = enum.get("type")  # May be None
    enum_kind = enum.get("kind")  # e.g., 'Value', 'BitEnum'
    
    header = f"# Enum: {enum_name}"
    details = " (" + ", ".join(
        x for x in [enum_category, enum_kind, enum_type] if x
    ) + ")"
    print(header + details)
    print("| Enumerant | Value | Comment |")
    print("|------------|--------|----------|")

    for enumerant in enum.findall("enum"):
        name = enumerant.get("name", "")
        value = enumerant.get("value") or enumerant.get("bitpos") or ""
        comment = clean(enumerant.get("comment"))
        print(f"| {name} | {value} | {comment} |")

    print()  # spacing between enums
