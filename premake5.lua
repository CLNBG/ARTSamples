group "HardwareVendorDemo/ART"
  include "PCI9757/VC/Advanced"
  include "ACTS1000/VC/Advanced"
  include "PCI8501/VC/Advanced"
  include "PCI8510/VC/Advanced"

	group "HardwareVendorDemo/ART/DAQ"
		include "DAQSamples"
	group""

	group "HardwareVendorDemo/ART/ArtSCOPE"
		include "SCOPESamples"
	group""
group ""
