import os

def update_config_file(filepath):
    """
    Updates the 'JetType' and 'JetCone' values within a configuration file.
    If the correct values are already set, the file is skipped.
    """
    try:
        with open(filepath, 'r') as f:
            lines = f.readlines()

        updated_lines = []
        needs_update = False
        jet_type_found = False
        jet_cone_found = False

        for line in lines:
            if "JetType :" in line:
                if 'JetType : "AK4PFchs"' not in line:
                    updated_lines.append('JetType : "AK4PFchs"\n')
                    needs_update = True
                else:
                    updated_lines.append(line)
                jet_type_found = True
            elif "JetCone :" in line:
                if 'JetCone : 0.4' not in line:
                    updated_lines.append('JetCone : 0.4\n')
                    needs_update = True
                else:
                    updated_lines.append(line)
                jet_cone_found = True
            else:
                updated_lines.append(line)
        
        # Check if JetType or JetCone lines were found in the file at all
        if not jet_type_found or not jet_cone_found:
            print(f"Warning: Could not find 'JetType' or 'JetCone' lines in {filepath}. Skipping this file.")
            return

        if needs_update:
            print(f"Updating file: '{filepath}'...")
            with open(filepath, 'w') as f:
                f.writelines(updated_lines)
            print(f"Update complete for '{filepath}'.")
        else:
            print(f"Skipping '{filepath}' as it already has the correct settings.")

    except Exception as e:
        print(f"Error processing file '{filepath}': {e}")

def main():
    """
    Main function to initiate the configuration file update process.
    It walks through the current directory and its subdirectories to find .config files.
    """
    # Start searching from the current directory.
    # Assumes the script is run from the top-level directory where the config files reside
    # (e.g., 'JECSummer20L2Relative_2018_v1').
    root_dir = "." 
    
    print("Starting configuration file update...\n")
    
    # Walk through all directories and subdirectories within root_dir
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            # Check if the file has a .config extension
            if filename.endswith(".config"):
                filepath = os.path.join(dirpath, filename)
                update_config_file(filepath)
    print("\nAll configuration files processed.")

if __name__ == "__main__":
    main()
