ARTSamplesBaseDir  = os.getcwd()

group "Hardware.Vendor.Demos/ART"
  include "PCI9757/VC/Advanced"
  include "ACTS1000/VC/Advanced"
  include "PCI8501/VC/Advanced"
  include "PCI8501/VC/Simple/AD"
  include "PCI8510/VC/Advanced"

	group "Hardware.Vendor.Demos/ART/DAQ"
		include "DAQSamples"
	group""

	group "Hardware.Vendor.Demos/ART/ArtSCOPE"
		include "SCOPESamples"
	group""
group ""
