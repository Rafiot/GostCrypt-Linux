/*
 Copyright (c) 2008 TrueCrypt Developers Association. All rights reserved.

 Governed by the TrueCrypt License 3.0 the full text of which is contained in
 the file License.txt included in TrueCrypt binary and source code distribution
 packages.
*/


#include "MountOptions.h"
#include "Platform/MemoryStream.h"
#include "Platform/SerializerFactory.h"

namespace GostCrypt
{
	void MountOptions::CopyFrom (const MountOptions &other)
	{
#define GST_CLONE(NAME) NAME = other.NAME
#define GST_CLONE_SHARED(TYPE,NAME) NAME = other.NAME ? make_shared <TYPE> (*other.NAME) : shared_ptr <TYPE> ()

		GST_CLONE (CachePassword);
		GST_CLONE (FilesystemOptions);
		GST_CLONE (FilesystemType);
		GST_CLONE_SHARED (KeyfileList, Keyfiles);
		GST_CLONE_SHARED (DirectoryPath, MountPoint);
		GST_CLONE (NoFilesystem);
		GST_CLONE (NoHardwareCrypto);
		GST_CLONE (NoKernelCrypto);
		GST_CLONE_SHARED (VolumePassword, Password);
		GST_CLONE_SHARED (VolumePath, Path);
		GST_CLONE (PartitionInSystemEncryptionScope);
		GST_CLONE (PreserveTimestamps);
		GST_CLONE (Protection);
		GST_CLONE_SHARED (VolumePassword, ProtectionPassword);
		GST_CLONE_SHARED (KeyfileList, ProtectionKeyfiles);
		GST_CLONE (Removable);
		GST_CLONE (SharedAccessAllowed);
		GST_CLONE (SlotNumber);
		GST_CLONE (UseBackupHeaders);
	}

	void MountOptions::Deserialize (shared_ptr <Stream> stream)
	{
		Serializer sr (stream);

		sr.Deserialize ("CachePassword", CachePassword);
		sr.Deserialize ("FilesystemOptions", FilesystemOptions);
		sr.Deserialize ("FilesystemType", FilesystemType);

		Keyfiles = Keyfile::DeserializeList (stream, "Keyfiles");

		if (!sr.DeserializeBool ("MountPointNull"))
			MountPoint.reset (new DirectoryPath (sr.DeserializeWString ("MountPoint")));
		else
			MountPoint.reset();

		sr.Deserialize ("NoFilesystem", NoFilesystem);
		sr.Deserialize ("NoHardwareCrypto", NoHardwareCrypto);
		sr.Deserialize ("NoKernelCrypto", NoKernelCrypto);

		if (!sr.DeserializeBool ("PasswordNull"))
			Password = Serializable::DeserializeNew <VolumePassword> (stream);
		else
			Password.reset();

		if (!sr.DeserializeBool ("PathNull"))
			Path.reset (new VolumePath (sr.DeserializeWString ("Path")));
		else
			Path.reset();

		sr.Deserialize ("PartitionInSystemEncryptionScope", PartitionInSystemEncryptionScope);
		sr.Deserialize ("PreserveTimestamps", PreserveTimestamps);

		Protection = static_cast <VolumeProtection::Enum> (sr.DeserializeInt32 ("Protection"));

		if (!sr.DeserializeBool ("ProtectionPasswordNull"))
			ProtectionPassword = Serializable::DeserializeNew <VolumePassword> (stream);
		else
			ProtectionPassword.reset();

		ProtectionKeyfiles = Keyfile::DeserializeList (stream, "ProtectionKeyfiles");
		sr.Deserialize ("Removable", Removable);
		sr.Deserialize ("SharedAccessAllowed", SharedAccessAllowed);
		sr.Deserialize ("SlotNumber", SlotNumber);
		sr.Deserialize ("UseBackupHeaders", UseBackupHeaders);
	}

	void MountOptions::Serialize (shared_ptr <Stream> stream) const
	{
		Serializable::Serialize (stream);
		Serializer sr (stream);

		sr.Serialize ("CachePassword", CachePassword);
		sr.Serialize ("FilesystemOptions", FilesystemOptions);
		sr.Serialize ("FilesystemType", FilesystemType);
		Keyfile::SerializeList (stream, "Keyfiles", Keyfiles);

		sr.Serialize ("MountPointNull", MountPoint == nullptr);
		if (MountPoint)
			sr.Serialize ("MountPoint", wstring (*MountPoint));

		sr.Serialize ("NoFilesystem", NoFilesystem);
		sr.Serialize ("NoHardwareCrypto", NoHardwareCrypto);
		sr.Serialize ("NoKernelCrypto", NoKernelCrypto);
		
		sr.Serialize ("PasswordNull", Password == nullptr);
		if (Password)
			Password->Serialize (stream);

		sr.Serialize ("PathNull", Path == nullptr);
		if (Path)
			sr.Serialize ("Path", wstring (*Path));

		sr.Serialize ("PartitionInSystemEncryptionScope", PartitionInSystemEncryptionScope);
		sr.Serialize ("PreserveTimestamps", PreserveTimestamps);
		sr.Serialize ("Protection", static_cast <uint32> (Protection));

		sr.Serialize ("ProtectionPasswordNull", ProtectionPassword == nullptr);
		if (ProtectionPassword)
			ProtectionPassword->Serialize (stream);

		Keyfile::SerializeList (stream, "ProtectionKeyfiles", ProtectionKeyfiles);
		sr.Serialize ("Removable", Removable);
		sr.Serialize ("SharedAccessAllowed", SharedAccessAllowed);
		sr.Serialize ("SlotNumber", SlotNumber);
		sr.Serialize ("UseBackupHeaders", UseBackupHeaders);
	}

	GST_SERIALIZER_FACTORY_ADD_CLASS (MountOptions);
}
